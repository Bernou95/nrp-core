/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#include "nrp_gz_grpc_engine/engine_server/gz_grpc_engine.h"

#include "nrp_general_library/utils/nrp_exceptions.h"

#include <sdf/Root.hh>
#include <sdf/World.hh>
#include <sdf/Model.hh>
#include <sdf/Physics.hh>

#include <csignal>
#include <iterator>
#include <sstream>

using namespace nlohmann;

namespace
{
    /*!
     * \brief Joins sdformat errors into a single message
     */
    std::string sdfErrorsToString(const sdf::Errors &errors)
    {
        std::string errStr;
        for(const auto &e : errors)
            errStr += "\n    " + e.Message();

        return errStr;
    }
}

GzGrpcEngine::GzGrpcEngine(const std::string &engineName,
                           const std::string &protobufPluginsPath,
                           const nlohmann::json &protobufPlugins) :
        EngineProtoWrapper(engineName, protobufPluginsPath, protobufPlugins)
{
    GzEngineSingleton::resetInstance(this);
}

GzGrpcEngine::~GzGrpcEngine()
{
    // Systems hold datapack controllers registered in this engine, destroy them first
    this->_server.reset();
}

gz::sim::ServerConfig GzGrpcEngine::createServerConfig(const json &data)
{
    const auto worldFile = data.at("GazeboWorldFile").get<std::string>();

    sdf::Root root;
    const auto loadErrors = root.Load(worldFile);
    if(!loadErrors.empty())
        throw NRPException::logCreate("Failed to load world file \"" + worldFile + "\":" + sdfErrorsToString(loadErrors));

    if(root.WorldCount() != 1)
        throw NRPException::logCreate("World file \"" + worldFile + "\" must contain exactly one world, it contains " +
                                      std::to_string(root.WorldCount()));

    sdf::World *world = root.WorldByIndex(0);

    // Add additional models
    if(data.contains("GazeboSDFModels")) {
        for (const auto &model: data.at("GazeboSDFModels")) {
            const auto modelName = model.at("Name").get<std::string>();
            const auto modelFile = model.at("File").get<std::string>();

            // Parse pose
            std::istringstream poseStr(model.at("InitPose").get<std::string>());
            std::vector<std::string> poseArgs(std::istream_iterator<std::string>{poseStr},
                                              std::istream_iterator<std::string>());
            if(poseArgs.size() != 6)
                throw NRPException::logCreate("Error while parsing SDF model" + modelName
                                              + ". Pose array must contain 6 elements, contains " + std::to_string(poseArgs.size()));

            const gz::math::Pose3d pose(std::stod(poseArgs[0]), std::stod(poseArgs[1]), std::stod(poseArgs[2]),
                                        std::stod(poseArgs[3]), std::stod(poseArgs[4]), std::stod(poseArgs[5]));

            sdf::Root modelRoot;
            const auto modelErrors = modelRoot.Load(modelFile);
            if(!modelErrors.empty() || modelRoot.Model() == nullptr)
                throw NRPException::logCreate("Failed to load model \"" + modelName + "\" from file \"" + modelFile + "\":" +
                                              sdfErrorsToString(modelErrors));

            sdf::Model newModel = *modelRoot.Model();
            newModel.SetName(modelName);
            newModel.SetRawPose(pose);
            newModel.SetPoseRelativeTo("");

            NRPLogger::info("Adding model \"{}\" from file \"{}\"", modelName, modelFile);
            if(!world->AddModel(newModel))
                throw NRPException::logCreate("Failed to add model \"" + modelName + "\". A model with the same name already exists");
        }

        const auto graphErrors = root.UpdateGraphs();
        if(!graphErrors.empty())
            throw NRPException::logCreate("Failed to add models to world file \"" + worldFile + "\":" + sdfErrorsToString(graphErrors));
    }

    // Find default physics profile
    sdf::Physics *physics = nullptr;
    for(uint64_t i = 0; i < world->PhysicsCount(); ++i) {
        if(physics == nullptr || world->PhysicsByIndex(i)->IsDefault())
            physics = world->PhysicsByIndex(i);
    }

    if(physics == nullptr)
        throw NRPException::logCreate("World file \"" + worldFile + "\" has no physics configuration");

    this->_stepSize = toSimulationTime<double, std::ratio<1>>(physics->MaxStepSize());

    // Steps are triggered by NRP, run them as fast as possible
    physics->SetRealTimeFactor(0);

    gz::sim::ServerConfig serverConfig;
    serverConfig.SetSdfRoot(root);
    serverConfig.SetSeed(data.at("GazeboRNGSeed").get<unsigned int>());
    serverConfig.SetHeadlessRendering(data.at("GzHeadlessRendering").get<bool>());

    return serverConfig;
}

SimulationTime GzGrpcEngine::runLoopStep(SimulationTime timeStep)
{
    NRP_LOGGER_TRACE("{} called [ timeStep: {} ]", __FUNCTION__, timeStep.count());

    if(this->_server == nullptr)
    {
        auto err = std::out_of_range("Tried to run loop while the engine has not yet been initialized");
        NRPLogger::error(err.what());

        throw err;
    }

    const unsigned numIterations = std::max(static_cast<unsigned int>(static_cast<double>(timeStep.count()) / static_cast<double>(this->_stepSize.count())), 1u);

    if(!this->_server->Run(true, numIterations, false))
        throw NRPException::logCreate("Error while executing gz-sim step");

    return this->_worldSystem->simTime();
}

void GzGrpcEngine::initialize(const json &data)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if(this->_server != nullptr)
        throw NRPException::logCreate("Engine \"" + this->getEngineName() + "\" has already been initialized");

    // The world is loaded synchronously. NRP gz-sim systems register their datapacks while the server is created
    // and on its first iteration
    this->_server.reset(new gz::sim::Server(this->createServerConfig(data)));

    // gz-sim installs a SIGTERM handler which only stops the simulation. Restore the default behavior, so that
    // the engine process terminates when it is stopped by NRP
    signal(SIGTERM, SIG_DFL);

    this->_worldSystem = std::make_shared<nrp_gz::NRPWorldSystem>();
    if(!this->_server->AddSystem(this->_worldSystem).value_or(false))
        throw NRPException::logCreate("Failed to add NRP world system to gz-sim server");

    // Run a single paused iteration. This finishes loading all entities and systems
    if(!this->_server->RunOnce(true))
        throw NRPException::logCreate("gz-sim server failed to finish loading world \"" +
                                      data.at("GazeboWorldFile").get<std::string>() + "\"");

    // Check that additional models were added
    if(data.contains("GazeboSDFModels")) {
        for (const auto &model: data.at("GazeboSDFModels")) {
            if(!this->_server->HasEntity(model.at("Name").get<std::string>()))
                throw NRPException::logCreate("Model \"" + model.at("Name").get<std::string>() + "\" was not added to the gz-sim simulation");
        }
    }

    NRPLogger::info("gz-sim world loaded. Registered datapacks: {}", this->getNumRegisteredDataPacks());

    this->_initRunFlag = true;
}

void GzGrpcEngine::reset()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if(this->_server == nullptr)
        throw NRPException::logCreate("Engine \"" + this->getEngineName() + "\" can't be reset before it is initialized");

    // Reset the world to its initial state. Systems implementing ISystemReset are reset during the next iteration,
    // and the world state is restored during the one after. Both iterations run paused
    this->_server->ResetAll();
    for(int i = 0; i < 2; ++i)
    {
        if(!this->_server->RunOnce(true))
            throw NRPException::logCreate("gz-sim server failed to reset");
    }
}

void GzGrpcEngine::shutdown()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    this->_server.reset();
    this->_shutdownFlag = true;
}

std::unique_ptr<GzEngineSingleton> GzEngineSingleton::_instance = nullptr;

GzEngineSingleton &GzEngineSingleton::getInstance()
{
    if(GzEngineSingleton::_instance)
        return *(GzEngineSingleton::_instance.get());
    else
        throw NRPException::logCreate("Attempting to access GzEngineSingleton singleton, but it has to be"
                                      " instantiated first");
}

GzEngineSingleton &GzEngineSingleton::resetInstance(GzGrpcEngine* engine)
{
    GzEngineSingleton::_instance.reset(new GzEngineSingleton(engine));
    return GzEngineSingleton::getInstance();
}

GzEngineSingleton::GzEngineSingleton(GzGrpcEngine* engine)
    : _engine(engine)
{}

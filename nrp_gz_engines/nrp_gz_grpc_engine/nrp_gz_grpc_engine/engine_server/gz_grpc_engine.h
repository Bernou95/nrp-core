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

#ifndef GZ_GRPC_ENGINE_H
#define GZ_GRPC_ENGINE_H

#include "nrp_gz_grpc_engine/config/gz_grpc_config.h"
#include "nrp_gz_grpc_engine/engine_server/nrp_world_system.h"

#include "nrp_grpc_engine_protocol/engine_server/engine_proto_wrapper.h"

#include <gz/sim/Server.hh>

#include <memory>

/*!
 * \brief Gazebo Harmonic (gz-sim) engine
 *
 * Implementation of EngineProtoWrapper which runs a gz-sim server in the engine process. The simulation is advanced
 * by GzGrpcEngine::runLoopStep, so no simulation step is executed outside of it. DataPacks are registered by the
 * NRP gz-sim systems (joint, link, model and camera controllers) included in the simulated world.
 */
class GzGrpcEngine
        : public EngineProtoWrapper
{
    public:

        GzGrpcEngine(const std::string &engineName,
                     const std::string &protobufPluginsPath,
                     const nlohmann::json &protobufPlugins);

        ~GzGrpcEngine() override;

        /*!
         * \brief Registers a datapack controller locking the datapack mutex
         *
         * \param[in] datapackName       Name of the datapack to be registered
         * \param[in] datapackController Pointer to the datapack controller object that's supposed to be
         *                             registered in the engine
         */
        void registerDataPackWithLock(const std::string & datapackName, ProtoDataPackController *interface)
        {
            lock_t lock(this->_datapackLock);
            this->registerDataPack(datapackName, interface);
        }

        /*!
         * \brief Create datapack name from the given model and sensor/joint/link
         * \param modelName Name of the parent model of the controlled object
         * \param objectName Name of the controlled object (sensor, joint, link, ...)
         * \return Returns datapack name
         */
        static std::string createDataPackName(const std::string &modelName, const std::string &objectName)
        {   return modelName + "::" + objectName;  }

        SimulationTime runLoopStep(SimulationTime timeStep) override;

        void initialize(const nlohmann::json &data) override;

        void reset() override;

        void shutdown() override;

        bool initRunFlag() const override { return this->_initRunFlag; }

        bool shutdownFlag() const override { return this->_shutdownFlag; }

    private:

        /*!
         * \brief Creates the server configuration from the engine configuration. Additional models
         * listed in "GazeboSDFModels" are added to the world before the server is created
         */
        gz::sim::ServerConfig createServerConfig(const nlohmann::json &data);

        /*!
         * \brief gz-sim server running the simulation
         */
        std::unique_ptr<gz::sim::Server> _server;

        /*!
         * \brief System tracking the simulation time of the world
         */
        std::shared_ptr<nrp_gz::NRPWorldSystem> _worldSystem;

        /*!
         * \brief Physics step size of the world
         */
        SimulationTime _stepSize = SimulationTime::zero();

        bool _initRunFlag = false;

        bool _shutdownFlag = false;

        mutex_t _datapackLock;
};

/*!
 * \brief Singleton class which stores a pointer to the GzGrpcEngine being used to manage the simulation
 *
 * so the NRP gz-sim systems can invoke it to register datapacks
 */
class GzEngineSingleton
{
public:

    /*! \brief Delete for singleton */
    GzEngineSingleton(const GzEngineSingleton &other) = delete;

    /*! \brief Delete for singleton */
    GzEngineSingleton &operator=(const GzEngineSingleton &other) = delete;

    /*! \brief Delete for singleton */
    GzEngineSingleton(GzEngineSingleton &&other) = delete;

    /*! \brief Delete for singleton */
    GzEngineSingleton &&operator=(GzEngineSingleton &&other) = delete;

    /*!
     * \brief Get singleton instance
     * \return Gets instance of GzEngineSingleton
     */
    static GzEngineSingleton& getInstance();

    /*!
     * \brief Reset
     */
    static GzEngineSingleton& resetInstance(GzGrpcEngine* engine);

    GzGrpcEngine& engine()
    {
        return *_engine;
    }

private:

    /*!
     * \brief Singleton instance of this class
     */
    static std::unique_ptr<GzEngineSingleton> _instance;

    /*!
     * \brief Pointer to the GzGrpcEngine
     */
    GzGrpcEngine* _engine;

    /*!
     * \brief Make private for singleton
     */
    GzEngineSingleton() = delete;

    /*!
     * \brief Constructor. Private for singleton
     */
    GzEngineSingleton(GzGrpcEngine* engine);
};

#endif // GZ_GRPC_ENGINE_H

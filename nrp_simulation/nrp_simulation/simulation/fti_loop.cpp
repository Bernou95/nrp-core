//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_simulation/simulation/fti_loop.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/datapack_interface/datapack.h"
#include "nrp_general_library/utils/python_error_handler.h"
#include "nrp_general_library/utils/time_utils.h"

#include "nrp_simulation/datapack_handle/tf_manager_handle.h"
#include "nrp_simulation/datapack_handle/computational_graph_handle.h"

#include <iostream>

// Helper function which reads simulation config and returns the correct DataPackProcessor
static DataPackProcessor* makeHandleFromConfig(jsonSharedPtr config, SimulationDataManager * simulationDataManager)
{
    std::string dev_p = config->at("DataPackProcessor").get<std::string>();
    bool spinROS = config->contains("ROSNode");
    bool slaveMode = config->at("SimulationLoop") == "EventLoop";
    if(dev_p == "cg" || slaveMode)
        return new ComputationalGraphHandle(simulationDataManager, slaveMode, spinROS);
    else if(dev_p == "tf")
        return new TFManagerHandle(simulationDataManager);
    else
        throw NRPException::logCreate("Unsupported DataPackProcessor: " + dev_p);
}


static SimulationLoopType retrieveLoopTypeFromConfig(jsonSharedPtr config)
{
    std::string typeString = config->at("FTILoopType").get<std::string>();

    return (typeString == "RL") ? SimulationLoopType::RL : SimulationLoopType::ORIGINAL;
}

/*!
 * @brief Checks if the actual engine time matched the predicted engine time
 *
 * @param predictedEngineTime
 * @param actualEngineTime
 */
static bool isEngineTimeOk(const SimulationTime & predictedEngineTime, const SimulationTime & actualEngineTime)
{
    if(predictedEngineTime == SimulationTime::zero())
    {
        return actualEngineTime == SimulationTime::zero();
    }

    const SimulationTime engineTimeDiffThreshold = predictedEngineTime / 10000;

    return abs((predictedEngineTime - actualEngineTime).count()) < engineTimeDiffThreshold.count();
}


static DataPackProcessor::engine_interfaces_t convertQueueToVector(const FTILoop::engine_queue_t & queue)
{
    DataPackProcessor::engine_interfaces_t enginesVector;

    for(auto const& [predictedEngineTime, engine] : queue)
    {
        enginesVector.push_back(engine);
    }

    return enginesVector;
}


static void runLoopStepAsyncGet(EngineClientInterfaceSharedPtr engine)
{
    const SimulationTime timeout = toSimulationTimeFromSeconds(engine->engineConfig().at("EngineCommandTimeout"));

    try
    {
        // Wait for run loop step completion and get the results

        engine->runLoopStepAsyncGet(timeout);
    }
    catch(std::exception &e)
    {
        throw NRPException::logCreate(e, "Error while executing runLoopStep in engine \"" + engine->engineName()
        + "\". The Engine didn't respond before timeout. Check the engine logs for errors.");
    }
}


static void runLoopStepAsync(const FTILoop::engine_queue_t & engines)
{
    for(auto const& [predictedEngineTime, engine] : engines)
    {
        try
        {
            // Execute run loop step in a worker thread
            // The results will be checked before running the transceiver functions

            engine->runLoopStepAsync(engine->getEngineTimestep());
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to start loop of engine \"" + engine->engineName() + "\"");
        }
    }
}


static void runLoopStepAsyncGet(const FTILoop::engine_queue_t & engines)
{
    for(auto const& [predictedEngineTime, engine] : engines)
    {
        runLoopStepAsyncGet(engine);
    }
}


FTILoop::FTILoop(jsonSharedPtr config, DataPackProcessor::engine_interfaces_t engines, SimulationDataManager * simulationDataManager, SimulationTime timestep)
    : _config(config),
      _engines(engines),
      _devHandler(makeHandleFromConfig(config, simulationDataManager)),
      _simTimestep(timestep),
      _loopType(retrieveLoopTypeFromConfig(config))
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
}

void FTILoop::initEngineQueue(const DataPackProcessor::engine_interfaces_t & engines)
{
    for(const auto &engine : engines)
    {
        this->_engineQueue.emplace(SimulationTime::zero(), engine);
    }
}

void FTILoop::initLoop()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Init DataPack handle
    this->_devHandler->init(_config, _engines);

    initEngineQueue(_engines);

    // Init Engine
    for(const auto &engine : this->_engines)
    {
        try
        {
            engine->initialize();
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to initialize engine \"" + engine->engineName() + "\"");
        }
    }

    this->_devHandler->postEngineInit(this->_engines);
}

FTILoop::engine_queue_t FTILoop::getEnginesForCurrentStep(SimulationTime timeUntil)
{
    FTILoop::engine_queue_t engines;

    while(!this->_engineQueue.empty() && this->_engineQueue.begin()->first <= timeUntil)
    {
        engines.emplace(this->_engineQueue.begin()->first, this->_engineQueue.begin()->second);
        this->_engineQueue.erase(this->_engineQueue.begin());
    }

    return engines;
}

void FTILoop::resetLoop()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // reset engine queue
    if (!this->_engineQueue.empty())
    {
        auto engines = getEnginesForCurrentStep(_simTime);

        for(auto const& [engineTime, engine] : engines)
        {
            runLoopStepAsyncGet(engine);
        }
    }

    this->_devHandler->preEngineReset(this->_engines);

    for(const auto &engine : this->_engines)
    {
        try
        {
            engine->reset();
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to reset engine \"" + engine->engineName() + "\"");
        }
    }

    initEngineQueue(_engines);

    this->_simTime = SimulationTime::zero();
    this->_devHandler->postEngineReset(this->_engines);
}

void FTILoop::shutdownLoop()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    for(const auto &engine : this->_engines)
    {
        try
        {
            engine->shutdown();
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to shutdown engine \"" + engine->engineName() + "\"");
        }
    }
}


void FTILoop::runLoopOriginal(FTILoop::engine_queue_t & engines)
{
    NRP_LOG_TIME_BLOCK("step_duration");

    DataPackProcessor::engine_interfaces_t enginesVector = convertQueueToVector(engines);

    runLoopStepAsyncGet(engines);

    NRP_LOG_TIME("after_run_loop_async_get");

    // Retrieve datapacks required by TFs from completed engines
    // Execute preprocessing TFs and TFs sequentially
    // Send tf output datapacks to corresponding engines
    this->_devHandler->datapackCycle(enginesVector);

    NRP_LOG_TIME("after_datapack_cycle");

    runLoopStepAsync(engines);

    NRP_LOG_TIME("after_run_loop_async");
}


void FTILoop::runLoopRL(FTILoop::engine_queue_t & engines)
{
    NRP_LOG_TIME_BLOCK("step_duration");

    DataPackProcessor::engine_interfaces_t enginesVector = convertQueueToVector(engines);

    this->_devHandler->sendDataPacks(enginesVector);

    NRP_LOG_TIME("after_send_datapacks");

    runLoopStepAsync(engines);

    NRP_LOG_TIME("after_run_loop_async");

    runLoopStepAsyncGet(engines);

    NRP_LOG_TIME("after_run_loop_async_get");

    this->_devHandler->getDataPacksAndRunFunctions(enginesVector);

    NRP_LOG_TIME("after_get_datapacks_and_run_funcs");
}


void FTILoop::runLoop(int numIterations)
{
    while(numIterations-- > 0)
    {
        this->_devHandler->setSimulationTime(this->_simTime);
        this->_devHandler->setSimulationIteration(this->_simIteration);

        auto engines = getEnginesForCurrentStep(this->_simTime);

        for(auto const& [predictedEngineTime, engine] : engines)
        {
            if(!isEngineTimeOk(predictedEngineTime, engine->getEngineTime()))
            {
                NRPLogger::warn("Actual engine time (" + std::to_string(engine->getEngineTime().count()) +
                                ") differs from predicted engine time (" + std::to_string(predictedEngineTime.count()) +
                                ") for engine " + engine->engineName());
            }
        }

        if(this->_loopType == SimulationLoopType::ORIGINAL)
        {
            runLoopOriginal(engines);
        }
        else  // SimulationLoopType::RL
        {
            runLoopRL(engines);
        }

        for(auto const& [predictedEngineTime, engine] : engines)
        {
            this->_engineQueue.emplace(predictedEngineTime + engine->getEngineTimestep(), engine);
        }

        this->_simIteration++;
        this->_simTime += this->_simTimestep;
    }
}


void FTILoop::waitForEngines()
{
    // Wait for all engines to finish their last loop step
    for(const auto &engine : this->_engineQueue)
    {
        runLoopStepAsyncGet(engine.second);
    }
}

// EOF

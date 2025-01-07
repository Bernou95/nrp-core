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

#include "edlut_grpc_engine/engine_server/edlut_grpc_server.h"
#include "edlut_grpc_engine/engine_server/edlut_spikes_datapack_controller.h"
#include "edlut_grpc_engine/engine_server/edlut_currents_datapack_controller.h"
#include <sstream>
#include <simulation/RealTimeRestriction.h>

// TODO: discuss sensorial_delay, output_delay, position control experiments

// Const values taken from original edlut baxter experiment, simulator_node2.cpp parameter values
// used in all launch files, can be make engine parameters
//const double OUTPUT_DELAY = 0.0;
//const double SENSORIAL_DELAY = 0.04;
//const double MAX_SIM_TIME_ADVANCE = SENSORIAL_DELAY * 0.9;
const float RT1_GAP = 0.7;
const float RT2_GAP = 0.9;
const float RT3_GAP = 0.95;

SimulationTime EdlutEngine::_simulationTime = SimulationTime::zero();

EdlutEngine::EdlutEngine(const std::string &engineName,
                                                   const std::string &protobufPluginsPath,
                                                   const nlohmann::json &protobufPlugins)
    : EngineProtoWrapper(engineName, protobufPluginsPath, protobufPlugins),
      _engineName(engineName)
{
    this->_inputSpikeDriver = std::make_shared<ArrayInputSpikeDriver>();
    this->_outputSpikeDriver = std::make_shared<ArrayOutputSpikeDriver>();
    this->_inputCurrentDriver = std::make_shared<ArrayInputCurrentDriver>();
}

SimulationTime EdlutEngine::runLoopStep(SimulationTime timeStep)
{

    NRP_LOGGER_TRACE("EdlutEngine::runLoopStep called");

    this->_simulationTime += timeStep;
    //NRPLogger::info("Sim time: "+std::to_string(fromSimulationTime<float, std::ratio<1>>(EdlutEngine::_simulationTime))+" rounded sim time: "+std::to_string((static_cast<double>(round(1000*fromSimulationTime<double, std::ratio<1>>(EdlutEngine::_simulationTime)))/1000)));

    /* Run edlut simulator */
    this->_edlutSimul->RunSimulationSlot((fromSimulationTime<double, std::ratio<1>>(EdlutEngine::_simulationTime)));

    return this->_simulationTime;
}

void EdlutEngine::initialize(const nlohmann::json &data)
{
    NRPLogger::info("Initializing EDLUT gRPC Engine");
    this->_engineConfig = data;
    NRPLogger::debug(this->_engineConfig.dump(4));

    initEdlutEngine(data);

    if(data.contains("HandleRTControl")) {
        // TODO: in the ROS code setting up the RT object is done before calling edlut InitSimulation, make sure it
        //  is not important
        auto step = this->_engineConfig.at("EngineTimestep").get<float>();
        this->_edlutSimul->RealTimeRestrictionObject->SetParameterWatchDog(step,
                                                                           (this->_sensorialDelay) * 0.9,
                                                                           RT1_GAP, RT2_GAP, RT3_GAP);
        this->_edlutSimul->RealTimeRestrictionObject->SetExternalClockOption();
        this->_edlutSimul->RealTimeRestrictionObject->SetSleepPeriod(0.001);

        _watchdog = std::async(&RealTimeRestriction::Watchdog, this->_edlutSimul->RealTimeRestrictionObject);

        this->_edlutSimul->RealTimeRestrictionObject->StartWatchDog();
    }
}

void EdlutEngine::initEdlutEngine(const nlohmann::json &data)
{
    // Create Edlut sim objects
    try{
        auto networkFile = data.at("NetworkFile").get<std::string>();
        auto weightFile = data.at("WeightsFile").get<std::string>();
        auto simTimestep = data.at("EngineTimestep").get<double>();
        auto numThreads = data.at("NumThreads").get<int>();
        this->_sensorialDelay = data.at("SensorialDelay").get<float>();
        this->_saveWeightsPeriod = data.at("SaveWeightsPeriod").get<float>();

        this->_edlutSimul = std::make_shared<Simulation> (networkFile.c_str(), weightFile.c_str(), std::numeric_limits<double>::max(), simTimestep, numThreads);

        // Initialize simulation
        this->_edlutSimul->AddInputSpikeDriver(this->_inputSpikeDriver.get());
        this->_edlutSimul->AddOutputSpikeDriver(this->_outputSpikeDriver.get());
        this->_edlutSimul->AddInputCurrentDriver(this->_inputCurrentDriver.get());

        std::string weight_file = "output_weight.dat";
        this->_edlutSimul->AddOutputWeightDriver(new FileOutputWeightDriver(weight_file.c_str()));
        if(this->_saveWeightsPeriod>0.0){
            this->_edlutSimul->SetSaveStep(this->_saveWeightsPeriod);
            this->_edlutSimul->GetQueue()->InsertEventWithSynchronization(new SaveWeightsEvent(this->_edlutSimul->GetSaveStep(), this->_edlutSimul.get()));
        }

        this->_edlutSimul->InitSimulation();
    }
    catch (EDLUTException Exc){
        throw NRPException::logCreate("Failed to initialize \""+this->_engineName+"\" Engine: " + Exc.GetErrorMsg());
    }

    // Register datapack
    this->registerDataPack("spikes_datapack", new EdlutSpikesDataPackController("spikes_datapack", this->_engineName, this->_edlutSimul, this->_inputSpikeDriver, this->_outputSpikeDriver));
    this->registerDataPack("currents_datapack", new EdlutCurrentsDataPackController("currents_datapack", this->_engineName, this->_edlutSimul, this->_inputCurrentDriver));
    NRPLogger::info("DataPacks \"spikes_datapack\" and \"currents_datapack\" were registered");

    EdlutEngine::_simulationTime = SimulationTime::zero();
    this->_initRunFlag = true;
}

void EdlutEngine::shutdown()
{
    NRPLogger::info("Shutting down EDLUT simulation");
    this->_edlutSimul->RealTimeRestrictionObject->StopWatchDog();
    if(_watchdog.valid())
        _watchdog.wait();

    if(this->_saveWeightsPeriod<0)
        this->_edlutSimul->SaveWeights();
    this->_shutdownFlag = true;
}

void EdlutEngine::reset()
{
    NRPLogger::info("Resetting EDLUT simulation");

    this->clearRegisteredDataPacks();
    this->initEdlutEngine(this->_engineConfig);
}

// NOTE: this is not used currently, left here for testing controlling RT from EventLoop
void EdlutEngine::handleRTDelta(const SimulationTime timeDelta)
{
    auto* rtRestrObj = this->_edlutSimul->RealTimeRestrictionObject;
    auto gap = fromSimulationTime<float, std::ratio<1>>(timeDelta);
    auto MAX_SIM_TIME_ADVANCE = (0.9*this->_sensorialDelay);

    if (gap>(1-RT1_GAP) * MAX_SIM_TIME_ADVANCE){
        //The simulation time is fine.
        rtRestrObj->RestrictionLevel = RealTimeRestrictionLevel::ALL_EVENTS_ENABLED;
    }
    else if (gap>(1-RT2_GAP) * MAX_SIM_TIME_ADVANCE){
        //The simulation time evolves a bit slow. Learning rules are disabled.
        rtRestrObj->RestrictionLevel = RealTimeRestrictionLevel::LEARNING_RULES_DISABLED;
    }
    else if (gap>(1-RT3_GAP) * MAX_SIM_TIME_ADVANCE){
        //The simulation time evolves slow. Learning rules and spike generation and processing are disabled.
        rtRestrObj->RestrictionLevel = RealTimeRestrictionLevel::SPIKES_DISABLED;
    }
    else{
        //The simulation time evolves very slow (near the real time boundary or slower than RT). All unessential events are disabled.
        rtRestrObj->RestrictionLevel = RealTimeRestrictionLevel::ALL_UNESSENTIAL_EVENTS_DISABLED;
    }
}

void EdlutEngine::rtClockUpdate(const SimulationTime newClock)
{
    NRPLogger::debug("Real time: "+std::to_string(fromSimulationTime<float, std::ratio<1>>(newClock))+" Sim time: "+std::to_string(fromSimulationTime<float, std::ratio<1>>(EdlutEngine::_simulationTime))+" Dif: "+std::to_string(fromSimulationTime<float, std::ratio<1>>(EdlutEngine::_simulationTime)-fromSimulationTime<float, std::ratio<1>>(newClock)));
    //NRPLogger::debug("Sim time: "+std::to_string(fromSimulationTime<float, std::ratio<1>>(EdlutEngine::_simulationTime)));
    //this->_edlutSimul->RealTimeRestrictionObject->NextStepWatchDog(fromSimulationTime<float, std::ratio<1>>(newClock) - OUTPUT_DELAY * 0.9);
    this->_edlutSimul->RealTimeRestrictionObject->NextStepWatchDog(fromSimulationTime<float, std::ratio<1>>(newClock));
    if(fromSimulationTime<float, std::ratio<1>>(newClock)-this->_lastRTClock>=10){
        this->_edlutSimul->RealTimeRestrictionObject->ShowLocalStatistics();
        this->_edlutSimul->RealTimeRestrictionObject->ShowGlobalStatistics();
        this->_lastRTClock = fromSimulationTime<float, std::ratio<1>>(newClock);
    }

}
// EOF

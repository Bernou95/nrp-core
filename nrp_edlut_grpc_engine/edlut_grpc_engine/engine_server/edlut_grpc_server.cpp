//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
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
#include "edlut_grpc_engine/engine_server/edlut_grpc_datapack_controller.h"
#include <sstream>


int EdlutGrpcServer::_iteration = 0;
SimulationTime EdlutGrpcServer::_simulationTime = SimulationTime::zero();

EdlutGrpcServer::EdlutGrpcServer(const std::string &serverAddress,
                                     const std::string &engineName,
                                                   const std::string &protobufPluginsPath,
                                                   const nlohmann::json &protobufPlugins)
    : EngineGrpcServer(serverAddress, engineName, protobufPluginsPath, protobufPlugins),
    _engineName(engineName)
{

}

SimulationTime EdlutGrpcServer::runLoopStep(SimulationTime timeStep)
{

    NRP_LOGGER_TRACE("EdlutGrpcServer::runLoopStep called");

    this->_iteration++;

    /* Run edlut simulator */

    this->_simulationTime += timeStep;

    return this->_simulationTime;
}

void EdlutGrpcServer::initialize(const nlohmann::json &data, EngineGrpcServer::lock_t & /*datapackLock*/)
{
    NRPLogger::info("Initializing EDLUT gRPC Engine");
    try{
        auto network_file = data.at("NetworkFile").dump();
        auto weight_file = data.at("WeightsFile").dump();
        auto simulation_timestep = data.at("EngineTimestep").dump();
        auto num_threads = data.at("NumThreads").dump();

        network_file.erase(network_file.begin());
        network_file.erase(network_file.end()-1);
        weight_file.erase(weight_file.begin());
        weight_file.erase(weight_file.end()-1);

        this->edlutSimul = std::make_shared<Simulation> (network_file.c_str(), weight_file.c_str(), std::numeric_limits<double>::max(), std::stod(simulation_timestep.c_str()), std::stoi(num_threads.c_str()));


    }
    catch (NRPException &e) {
        //throw NRPException::logCreate( e.what());
        throw NRPException::logCreate("Unable to start EDLUT simulation");
    }


    auto &dumps = data.at("dumps");


    for(auto &dump : dumps){
        const auto datapackName = dump.at("name");
        _dataPacksNames.push_back(datapackName);
        this->registerDataPackNoLock(datapackName, new EdlutGrpcDataPackController(datapackName, this->_engineName, this->edlutSimul, &this->input_spike_driver, &this->output_spike_driver));

        NRPLogger::info("DataPack {} dump was added", datapackName);
    }

    /*Simul.AddInputSpikeDriver(new ROSTopicInputDriver(input_topic, max_spike_buffered, InitSimulationRosTime.toSec()));
    ROSTopicOutputDriver *output_driver = new ROSTopicOutputDriver(output_topic, max_spike_buffered, InitSimulationRosTime.toSec(), motor_delay);
    Simul.AddOutputSpikeDriver(output_driver);
    Simul.AddOutputWeightDriver(new FileOutputWeightDriver(weight_file));
    if (save_weight_period>0.0){
        Simul.SetSaveStep(save_weight_period);
        // Add the first save weight event
        Simul.GetQueue()->InsertEventWithSynchronization(new SaveWeightsEvent(Simul.GetSaveStep(), &Simul));
    }*/
    this->edlutSimul->AddInputSpikeDriver(&this->input_spike_driver);
    this->edlutSimul->AddOutputSpikeDriver(&this->output_spike_driver);

    this->edlutSimul->InitSimulation();

    this->_initRunFlag = true;

}

void EdlutGrpcServer::shutdown(const nlohmann::json &/*data*/)
{
    std::cout << "Shutting down simulation" << std::endl;
    this->_shutdownFlag = true;
}

void EdlutGrpcServer::reset()
{
    std::cout << "Resetting simulation" << std::endl;
}


// EOF

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

    vector<double> event_time;
    vector<long int> neuron_index;
    /* Run edlut simulator */
    for(auto& name: _dataPacksNames)
    {
        auto dpc = this->getDataPackController(name);

    }
    //this->_handleDataPackMessage
    this->addExternalSpikeActivity(event_time,neuron_index);
    this->edlutSimul->RunSimulationSlot(fromSimulationTime<double,ratio<1>>(this->_simulationTime));
    //output_driver->SendSpikeGroup();
    this->getSpikeActivity(event_time,neuron_index);

    this->_simulationTime += timeStep;

    return this->_simulationTime;
}

void EdlutGrpcServer::initialize(const nlohmann::json &data, EngineGrpcServer::lock_t & /*datapackLock*/)
{
    //const auto datapackName = "test_datapack";

    NRPLogger::info("Initializing EDLUT gRPC Engine");
    try{
        auto network_file = data.at("NetworkFile").dump();
        auto weight_file = data.at("WeightsFile").dump();
        auto simulation_timestep = data.at("EngineTimestep").dump();
        auto num_threads = data.at("NumThreads").dump();
        //NRPLogger::info("Network: "+network_file+" Weights: "+weight_file+ " SimTimestep: "+simulation_timestep+" Threads: "+num_threads);
        network_file.erase(network_file.begin());
        network_file.erase(network_file.end()-1);
        weight_file.erase(weight_file.begin());
        weight_file.erase(weight_file.end()-1);
        NRPLogger::info(network_file);
        //NRPLogger::info("Network: "+network_file+" Weights: "+weight_file+ " SimTimestep: "+simulation_timestep+" Threads: "+num_threads);
        this->edlutSimul = std::make_shared<Simulation> (network_file.c_str(), weight_file.c_str(), std::numeric_limits<double>::max(), std::stod(simulation_timestep.c_str()), std::stoi(num_threads.c_str()));
        //Simulation edlutSimul("Network_2.cfg", "Weights_2.cfg", std::numeric_limits<double>::max(), 0.002, 4);

    }
    catch (NRPException &e) {
        //throw NRPException::logCreate( e.what());
        throw NRPException::logCreate("Unable start EDLUT simulation");
    }


    auto &dumps = data.at("dumps");


    for(auto &dump : dumps){
        const auto datapackName = dump.at("name");
        _dataPacksNames.push_back(datapackName);
        this->registerDataPackNoLock(datapackName, new EdlutGrpcDataPackController(datapackName, this->_engineName));

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

void EdlutGrpcServer::addExternalSpikeActivity(const std::vector<double> & event_time, const std::vector<long int> & neuron_index) noexcept(false){
    try{
        if (!this->_initRunFlag) {
            throw EDLUTException(TASK_INPUT_SPIKE_DRIVER, ERROR_NON_INITIALIZED_SIMULATION, REPAIR_EXECUTE_AFTER_INITIALIZE_SIMULATION);
        }

        //we introduce the new activity in the driver.
        if (event_time.size()>0) {
            this->input_spike_driver.LoadInputs(this->edlutSimul->GetQueue(), this->edlutSimul->GetNetwork(), event_time.size(), &event_time[0], &neuron_index[0]);
        }
    }
    catch (EDLUTException Exc){
        cerr << Exc << endl;
        throw EDLUTException(TASK_EDLUT_INTERFACE, ERROR_EDLUT_INTERFACE, REPAIR_EDLUT_INTERFACE);
    }
}

void EdlutGrpcServer::getSpikeActivity(std::vector<double> & event_time, std::vector<long int> & neuron_index) noexcept(false){
    try{
        if (!this->_initRunFlag) {
            throw EDLUTException(TASK_OUTPUT_SPIKE_DRIVER, ERROR_NON_INITIALIZED_SIMULATION, REPAIR_EXECUTE_AFTER_INITIALIZE_SIMULATION);
    }

    double * OutputSpikeTimes;
    long int * OutputSpikeCells;

    unsigned int OutputSpikes = this->output_spike_driver.GetBufferedSpikes(OutputSpikeTimes,OutputSpikeCells);

    if (OutputSpikes>0) {
        event_time.resize(OutputSpikes);
        neuron_index.resize(OutputSpikes);

        double * SpTimesPtr = OutputSpikeTimes;
        long int * SpCellsPtr = OutputSpikeCells;
        std::vector<double>::iterator itTimes = event_time.begin();
        std::vector<long int>::iterator itNeurons = neuron_index.begin();

        for (unsigned int counter=0; counter<OutputSpikes; ++counter,++SpTimesPtr, ++SpCellsPtr, ++itTimes, ++itNeurons) {
            *itTimes = *SpTimesPtr;
            *itNeurons = *SpCellsPtr;
        }
    }

    return;
    }
    catch (EDLUTException Exc){
        cerr << Exc << endl;
        throw EDLUTException(TASK_EDLUT_INTERFACE, ERROR_EDLUT_INTERFACE, REPAIR_EDLUT_INTERFACE);
    }
}

// EOF

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

#include "edlut_grpc_engine/engine_server/edlut_grpc_datapack_controller.h"

EdlutGrpcDataPackController::EdlutGrpcDataPackController(const std::string & datapackName,
                                                         const std::string & engineName,
                                                         const std::shared_ptr<Simulation> edlutSim,
                                                         ArrayInputSpikeDriver* isd,
                                                         ArrayOutputSpikeDriver* osd)
    : _datapackName(datapackName), _engineName(engineName), _edlutSimul(edlutSim), _input_spike_driver(isd), _output_spike_driver(osd)
{

}

void EdlutGrpcDataPackController::handleDataPackData(const google::protobuf::Message &data)
{
    // In order to access the data from the message, you need to cast it to the proper type

    if (!this->_initialized) {
        auto msg = data.GetTypeName();


        // Check message type case. Only relevant for file dump
        if (msg == "Edlut.Spikes") {
            this->_initialized = true;
        }

    }

    // In order to access the data from the message, you need to cast it to the proper type
    if (this->_initialized == true && data.GetTypeName()=="Edlut.Spikes")
    {
        std::stringstream m_data;
        m_data <<"Simulation time EDLUT "<< fromSimulationTime<float, std::ratio<1>>(EdlutGrpcServer::_simulationTime) <<std::endl;

        auto stamp = data.GetReflection()->GetFloat(data, data.GetDescriptor()->FindFieldByName("time"));

        if(data.GetReflection()->FieldSize(data,data.GetDescriptor()->FindFieldByName("spikes_time"))> 0){
            for(int j=0;j<data.GetReflection()->FieldSize(data,data.GetDescriptor()->FindFieldByName("spikes_time"));j++)
            {
                this->event_time.push_back(data.GetReflection()->GetRepeatedFloat(data,data.GetDescriptor()->FindFieldByName("spikes_time"),j));
            }
        }
        if(data.GetReflection()->FieldSize(data,data.GetDescriptor()->FindFieldByName("neuron_indexes"))> 0)
        {
            for(int j=0;j<data.GetReflection()->FieldSize(data,data.GetDescriptor()->FindFieldByName("neuron_indexes"));j++)
            {
                this->neuron_index.push_back(data.GetReflection()->GetRepeatedUInt32(data,data.GetDescriptor()->FindFieldByName("neuron_indexes"),j));
            }
        }


        NRPLogger::debug(m_data.str());

        this->addExternalSpikeActivity(this->event_time,this->neuron_index);

        this->_edlutSimul->RunSimulationSlot(static_cast<double>(stamp));

        this->getSpikeActivity(this->event_time,this->neuron_index);

    }

}

google::protobuf::Message * EdlutGrpcDataPackController::getDataPackInformation()
{
    // Create a new protobuf message and fill it

    auto payload = new Edlut::Spikes();
    payload->set_time(fromSimulationTime<float, std::ratio<1>>(EdlutGrpcServer::_simulationTime));

    for(auto &spike: this->event_time)
    {
        payload->add_spikes_time(spike);
    }
    for(auto &index: this->neuron_index)
    {
        payload->add_neuron_indexes(index);
    }

    this->neuron_index.clear();
    this->event_time.clear() ;

    // Return the protobuf message

    return payload;
}

void EdlutGrpcDataPackController::addExternalSpikeActivity(const std::vector<double> & event_time, const std::vector<long int> & neuron_index) noexcept(false){
    try{
        if (!this->_initialized) {
        throw EDLUTException(TASK_INPUT_SPIKE_DRIVER, ERROR_NON_INITIALIZED_SIMULATION, REPAIR_EXECUTE_AFTER_INITIALIZE_SIMULATION);
        }

        //we introduce the new activity in the driver.
        if (event_time.size()>0) {
            this->_input_spike_driver->LoadInputs(this->_edlutSimul->GetQueue(), this->_edlutSimul->GetNetwork(), event_time.size(), &event_time[0], &neuron_index[0]);
        }
    }
    catch (EDLUTException Exc){
        cerr << Exc << endl;
        throw EDLUTException(TASK_EDLUT_INTERFACE, ERROR_EDLUT_INTERFACE, REPAIR_EDLUT_INTERFACE);
    }
}

void EdlutGrpcDataPackController::getSpikeActivity(std::vector<double> & event_time, std::vector<long int> & neuron_index) noexcept(false){
    try{
        if (!this->_initialized) {
            throw EDLUTException(TASK_OUTPUT_SPIKE_DRIVER, ERROR_NON_INITIALIZED_SIMULATION, REPAIR_EXECUTE_AFTER_INITIALIZE_SIMULATION);
        }

        double * OutputSpikeTimes;
        long int * OutputSpikeCells;

        unsigned int OutputSpikes = this->_output_spike_driver->GetBufferedSpikes(OutputSpikeTimes,OutputSpikeCells);

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

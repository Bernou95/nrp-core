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
                                                         const std::string & engineName)
    : _datapackName(datapackName), _engineName(engineName)
{

}

void EdlutGrpcDataPackController::handleDataPackData(const google::protobuf::Message &data)
{
    // In order to access the data from the message, you need to cast it to the proper type

    //std::cout << "TF output data: " << dynamic_cast<const Edlut::Spikes &>(data).clear_time() << std::endl;

    if (!this->_initialized) {
        auto msg = data.GetTypeName();


        // Check message type case. Only relevant for file dump
        if (msg == "Edlut.Spikes") {
            std::cout << "Todo OK: " << std::endl;
        }
        else {
            std::cout << "No todo OK: " << std::endl;
        }

        this->_initialized = true;
    }

    // In order to access the data from the message, you need to cast it to the proper type
    if (this->_initialized == true)
    {
        std::stringstream m_data;
        m_data << fromSimulationTime<float, std::ratio<1>>(EdlutGrpcServer::_simulationTime) <<std::endl;//",";
        auto n = data.GetDescriptor()->field_count();
        //std::cout << n <<std::endl;
        for(int i = 0; i < n; ++i){
            if(i==0)
            {
                //this->data.set_timestep(proto_field_ops::GetScalarField(data, data.GetDescriptor()->field(i))) ;
                this->data.set_timestep(data.GetReflection()->GetFloat(data, data.GetDescriptor()->field(i))) ;
            }
            if(i==1)
            {
                //this->data.set_time(proto_field_ops::GetScalarField(data, data.GetDescriptor()->field(i))) ;
                this->data.set_time(data.GetReflection()->GetFloat(data, data.GetDescriptor()->field(i))) ;
            }
            if(i==2 && data.GetReflection()->FieldSize(data,data.GetDescriptor()->FindFieldByName("spikes_time"))> 0){
                for(int j=0;j<data.GetReflection()->FieldSize(data,data.GetDescriptor()->FindFieldByName("spikes_time"));j++)
                {
                    //this->data.add_spikes_time(proto_field_ops::GetRepeatedFloat(data,data.GetDescriptor()->FindFieldByName("spikes_time"),j));
                    this->data.add_spikes_time(data.GetReflection()->GetRepeatedFloat(data,data.GetDescriptor()->FindFieldByName("spikes_time"),j));
                }
                //std::cout << data.GetReflection()->GetRepeatedFloat(data,data.GetDescriptor()->FindFieldByName("spikes_time"),1)<<" hola"<<std::endl;
            }
            if(i==3 && data.GetReflection()->FieldSize(data,data.GetDescriptor()->FindFieldByName("neuron_indexes"))> 0)
            {
                for(int j=0;j<data.GetReflection()->FieldSize(data,data.GetDescriptor()->FindFieldByName("neuron_indexes"));j++)
                {
                    //this->data.add_spikes_time(proto_field_ops::GetRepeatedScalarField(data,data.GetDescriptor()->FindFieldByName("neuron_indexes"),j));
                    this->data.add_spikes_time(data.GetReflection()->GetRepeatedUInt32(data,data.GetDescriptor()->FindFieldByName("neuron_indexes"),j));
                }
            }
                    //m_data << proto_field_ops::GetScalarFieldAsString(data, data.GetDescriptor()->field(i)) << ",";
                    //std::cout << data.GetReflection()->GetRepeatedFloat(data,data.GetDescriptor()->field(i),0)<<std::endl;

            }

        std::cout << m_data.str();

    }

}

google::protobuf::Message * EdlutGrpcDataPackController::getDataPackInformation()
{
    //static int data = 0;

    // Create a new protobuf message and fill it

    auto payload = new Edlut::Spikes(this->data);
    //read from this->data;
    //payload->set_integer(data++);
    //payload->spikes_time(0);
    payload->add_spikes_time(1.0);
    payload->add_spikes_time(2.0);
    //this->data.set_integer(data);

    // Return the protobuf message

    return payload;
}

Edlut::Spikes EdlutGrpcDataPackController::getData()
{
    return this->data;
}

// EOF

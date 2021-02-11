//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020 Michael Zechmair
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

#include "nrp_gazebo_grpc_engine/devices/grpc_dummy_device.h"

template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serialize<DummyDevice>(const DummyDevice &dev)
{
    static const std::string str = "test";

    GRPCDevice msg = serializeID<GRPCDevice>(dev.id());

    msg.dev().mutable_dummy()->InitAsDefaultInstance();

    msg.dev().mutable_dummy()->set_scalar(dev.getScalar());

    msg.dev().mutable_dummy()->add_array(dev.getArray()[0]);
    msg.dev().mutable_dummy()->add_array(dev.getArray()[1]);

    msg.dev().mutable_dummy()->set_string(str);

    //msg.dev().mutable_dummy()->vector(dev.getVector());
    *msg.dev().mutable_dummy()->mutable_vector() = {dev.getVector().begin(), dev.getVector().end()};

    return msg;
}

template<>
DummyDevice DeviceSerializerMethods<GRPCDevice>::deserialize<DummyDevice>(DeviceIdentifier &&devID, deserialization_t data)
{
    std::cout << "DESERIALIZE" << std::endl;
    return DummyDevice(std::move(devID), DummyDevice::property_template_t(data->dummy().scalar(),
                                                                          DummyDevicePropSpec::DummyArray({data->dummy().array(0), data->dummy().array(1)}),
                                                                          data->dummy().string(),
                                                                          DummyDevicePropSpec::DummyVector(data->dummy().vector().begin(),
	                                                                                                       data->dummy().vector().end())));
}

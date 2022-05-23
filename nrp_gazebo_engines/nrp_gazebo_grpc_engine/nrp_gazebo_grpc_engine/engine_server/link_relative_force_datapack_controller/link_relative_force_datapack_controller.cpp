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

#include "nrp_gazebo_grpc_engine/engine_server/link_relative_force_datapack_controller/link_relative_force_datapack_controller.h"
#include <ignition/math/Vector3.hh>
ForceGrpcDataPackController::ForceGrpcDataPackController(const std::string &linkName, const gazebo::physics::LinkPtr &link)
                : _name(linkName),
                  _link(link)
            {}


void ForceGrpcDataPackController::handleDataPackData(const google::protobuf::Message &data)
{
    // throws bad_cast

        const auto &j = dynamic_cast<const Gazebo::SetRelativeForce &>(data);


        if(!std::isnan(j.x()) && !std::isnan(j.y()) && !std::isnan(j.z())){
            ignition::math::Vector3d my_force_input = ignition::math::Vector3d(j.x(), j.y(), j.z());
            this->_link->AddRelativeForce(my_force_input);
        }


}

google::protobuf::Message * ForceGrpcDataPackController::getDataPackInformation(){
    auto l = new Gazebo::Link();

    const auto &pose = this->_link->WorldCoGPose();
    l->add_position(ToFloat(pose.Pos().X()));
    l->add_position(ToFloat(pose.Pos().Y()));
    l->add_position(ToFloat(pose.Pos().Z()));

    l->add_rotation(ToFloat(pose.Rot().X()));
    l->add_rotation(ToFloat(pose.Rot().Y()));
    l->add_rotation(ToFloat(pose.Rot().Z()));
    l->add_rotation(ToFloat(pose.Rot().W()));

    const auto &linVel = this->_link->WorldLinearVel();
    l->add_linearvelocity(ToFloat(linVel.X()));
    l->add_linearvelocity(ToFloat(linVel.Y()));
    l->add_linearvelocity(ToFloat(linVel.Z()));

    const auto &angVel = this->_link->WorldAngularVel();
    l->add_angularvelocity(ToFloat(angVel.X()));
    l->add_angularvelocity(ToFloat(angVel.Y()));
    l->add_angularvelocity(ToFloat(angVel.Z()));

    return l;
}
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

#include "nrp_gazebo_grpc_engine/engine_server/joint_set_velocity_datapack/joint_set_velocity_datapack.h"


JointGrpcDataPackController::JointGrpcDataPackController(const std::string &jointName, const gazebo::physics::JointPtr &joint)
            : _name(jointName),
              _joint(joint)
        {
        }

   void JointGrpcDataPackController::handleDataPackData(const google::protobuf::Message &data)
        {
            // throws bad_cast
            const auto &j = dynamic_cast<const Gazebo::SetVelocity &>(data);

            if(!std::isnan(j.velocity()))
                this->_joint->SetVelocity(0, j.velocity());

        }        

    google::protobuf::Message *JointGrpcDataPackController::getDataPackInformation()
        {
            auto l = new Gazebo::SetVelocity();

            l->set_velocity(this->_joint->GetVelocity(0));

            return l;
        }
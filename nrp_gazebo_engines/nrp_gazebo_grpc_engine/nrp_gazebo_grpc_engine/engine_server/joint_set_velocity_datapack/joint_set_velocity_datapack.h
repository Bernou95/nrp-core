/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
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

#ifndef JOINT_GRPC_DATAPACK_CONTROLLER_H
#define JOINT_GRPC_DATAPACK_CONTROLLER_H

#include <gazebo/gazebo.hh>
#include <gazebo/physics/Joint.hh>
#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_protobuf/gazebo_msgs.pb.h"

    /*!
     * \brief Interface for a single joint
     */
    class JointGrpcDataPackController
        : public DataPackController<google::protobuf::Message>
    {
    public:
        JointGrpcDataPackController(const std::string &jointName, const gazebo::physics::JointPtr &joint);

        virtual void handleDataPackData(const google::protobuf::Message &data) override;

        virtual google::protobuf::Message *getDataPackInformation() override;
    private:
        /*!
         * \brief Joint Name
         */
        std::string _name;

        /*!
         * \brief Pointer to joint
         */
        gazebo::physics::JointPtr _joint = nullptr;
    };

#endif // JOINT_GRPC_DATAPACK_CONTROLLER_H

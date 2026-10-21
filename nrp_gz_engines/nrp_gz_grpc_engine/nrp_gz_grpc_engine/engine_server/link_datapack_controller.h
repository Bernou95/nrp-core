/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef GZ_LINK_GRPC_DATAPACK_CONTROLLER_H
#define GZ_LINK_GRPC_DATAPACK_CONTROLLER_H

#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_protobuf/gazebo.pb.h"

#include <gz/math/Pose3.hh>
#include <gz/math/Vector3.hh>

#include <mutex>

namespace nrp_gz
{
    /*!
     * \brief Interface for links. The link state is updated by NRPLinkController after every simulation step
     */
    class LinkGrpcDataPackController
            : public DataPackController<google::protobuf::Message>
    {
            template<class T>
            constexpr static float ToFloat(const T &val)
            {   return static_cast<float>(val); }

        public:
            LinkGrpcDataPackController(const std::string &linkName)
                : _name(linkName)
            {}

            virtual void handleDataPackData(const google::protobuf::Message &) override
            {}

            virtual google::protobuf::Message *getDataPackInformation() override
            {
                auto l = new Gazebo::Link();

                std::scoped_lock lock(this->_mutex);

                const auto &pose = this->_pose;
                l->add_position(ToFloat(pose.Pos().X()));
                l->add_position(ToFloat(pose.Pos().Y()));
                l->add_position(ToFloat(pose.Pos().Z()));

                l->add_rotation(ToFloat(pose.Rot().X()));
                l->add_rotation(ToFloat(pose.Rot().Y()));
                l->add_rotation(ToFloat(pose.Rot().Z()));
                l->add_rotation(ToFloat(pose.Rot().W()));

                const auto &linVel = this->_linearVelocity;
                l->add_linearvelocity(ToFloat(linVel.X()));
                l->add_linearvelocity(ToFloat(linVel.Y()));
                l->add_linearvelocity(ToFloat(linVel.Z()));

                const auto &angVel = this->_angularVelocity;
                l->add_angularvelocity(ToFloat(angVel.X()));
                l->add_angularvelocity(ToFloat(angVel.Y()));
                l->add_angularvelocity(ToFloat(angVel.Z()));

                return l;
            }

            /*!
             * \brief Updates the link state sent to NRP
             * \param pose World pose of the link center of mass
             * \param linearVelocity Linear velocity of the link in world frame
             * \param angularVelocity Angular velocity of the link in world frame
             */
            void updateState(const gz::math::Pose3d &pose, const gz::math::Vector3d &linearVelocity, const gz::math::Vector3d &angularVelocity)
            {
                std::scoped_lock lock(this->_mutex);
                this->_pose = pose;
                this->_linearVelocity = linearVelocity;
                this->_angularVelocity = angularVelocity;
            }

        private:
            /*!
             * \brief Link Name
             */
            std::string _name;

            /*!
             * \brief Protects the link state
             */
            std::mutex _mutex;

            gz::math::Pose3d _pose;
            gz::math::Vector3d _linearVelocity;
            gz::math::Vector3d _angularVelocity;
    };
}

#endif // GZ_LINK_GRPC_DATAPACK_CONTROLLER_H

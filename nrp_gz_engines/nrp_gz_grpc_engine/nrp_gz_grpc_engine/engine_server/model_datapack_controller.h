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

#ifndef GZ_MODEL_GRPC_DATAPACK_CONTROLLER_H
#define GZ_MODEL_GRPC_DATAPACK_CONTROLLER_H

#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_protobuf/gazebo.pb.h"

#include <gz/math/Pose3.hh>
#include <gz/math/Vector3.hh>

#include <mutex>
#include <optional>

namespace nrp_gz
{
    /*!
     * \brief Interface for models
     *
     * The model state is updated by NRPModelController after every simulation step. The commands received from NRP
     * are applied by NRPModelController before the next step.
     */
    class ModelGrpcDataPackController
            : public DataPackController<google::protobuf::Message>
    {
            template<class T>
            constexpr static float ToFloat(const T &val)
            {   return static_cast<float>(val); }

        public:

            /*!
             * \brief Commands received from NRP since the last simulation step. Velocities are in world frame
             */
            struct Commands
            {
                std::optional<gz::math::Pose3d> pose;
                std::optional<gz::math::Vector3d> linearVelocity;
                std::optional<gz::math::Vector3d> angularVelocity;
            };

            ModelGrpcDataPackController(const std::string &modelName)
                : _name(modelName)
            {}

            virtual void handleDataPackData(const google::protobuf::Message &data) override
            {
                // throws bad_cast
                const auto &l = dynamic_cast<const Gazebo::Model &>(data);

                std::scoped_lock lock(this->_mutex);

                // Set Pose
                if(l.position_size() || l.rotation_size()) {
                    auto pose = this->_commands.pose.value_or(this->_pose);

                    if(l.position_size() && l.position_size() == 3) {
                        pose.Pos().X(l.position().Get(0));
                        pose.Pos().Y(l.position().Get(1));
                        pose.Pos().Z(l.position().Get(2));
                    }
                    else if(l.position_size())
                        throw NRPException::logCreate("Model msg position wrong");

                    if(l.rotation_size() && l.rotation_size() == 4) {
                        // Rotation is sent as x, y, z, w
                        pose.Rot() = gz::math::Quaterniond(l.rotation().Get(3), l.rotation().Get(0),
                                                           l.rotation().Get(1), l.rotation().Get(2));
                    }
                    else if(l.rotation_size())
                        throw NRPException::logCreate("Model msg rotation wrong");

                    this->_commands.pose = pose;
                }

                // Set Linear velocity
                if(l.linearvelocity_size() && l.linearvelocity_size() == 3)
                    this->_commands.linearVelocity = gz::math::Vector3d(l.linearvelocity().Get(0),
                                                                        l.linearvelocity().Get(1),
                                                                        l.linearvelocity().Get(2));
                else if(l.linearvelocity_size())
                    throw NRPException::logCreate("Model msg linear velocity wrong");

                // Set Angular velocity
                if(l.angularvelocity_size() && l.angularvelocity_size() == 3)
                    this->_commands.angularVelocity = gz::math::Vector3d(l.angularvelocity().Get(0),
                                                                         l.angularvelocity().Get(1),
                                                                         l.angularvelocity().Get(2));
                else if(l.angularvelocity_size())
                    throw NRPException::logCreate("Model msg angular velocity wrong");
            }

            virtual google::protobuf::Message *getDataPackInformation() override
            {
                auto l = new Gazebo::Model();

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
             * \brief Returns the commands received since the last call and clears them
             */
            Commands takeCommands()
            {
                std::scoped_lock lock(this->_mutex);
                Commands commands = this->_commands;
                this->_commands = Commands();

                return commands;
            }

            /*!
             * \brief Updates the model state sent to NRP
             * \param pose World pose of the model
             * \param linearVelocity Linear velocity of the model canonical link in world frame
             * \param angularVelocity Angular velocity of the model canonical link in world frame
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
             * \brief Model Name
             */
            std::string _name;

            /*!
             * \brief Protects the model state and the commands
             */
            std::mutex _mutex;

            /*!
             * \brief Pending commands
             */
            Commands _commands;

            gz::math::Pose3d _pose;
            gz::math::Vector3d _linearVelocity;
            gz::math::Vector3d _angularVelocity;
    };
}

#endif // GZ_MODEL_GRPC_DATAPACK_CONTROLLER_H

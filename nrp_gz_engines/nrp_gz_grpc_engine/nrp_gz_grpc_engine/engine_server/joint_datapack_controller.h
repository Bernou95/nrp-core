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

#ifndef GZ_JOINT_GRPC_DATAPACK_CONTROLLER_H
#define GZ_JOINT_GRPC_DATAPACK_CONTROLLER_H

#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_protobuf/gazebo.pb.h"

#include <cmath>
#include <mutex>
#include <optional>

namespace nrp_gz
{
    /*!
     * \brief Interface for a single joint
     *
     * gz-sim entities can only be accessed from within the simulation update loop. This controller therefore stores
     * the joint state, updated by NRPJointController after every step, and the commands received from NRP, which are
     * applied by NRPJointController before the next step.
     */
    class JointGrpcDataPackController
            : public DataPackController<google::protobuf::Message>
    {
        public:

            /*!
             * \brief Commands received from NRP since the last simulation step
             */
            struct Commands
            {
                std::optional<double> position;
                std::optional<double> velocity;
                std::optional<double> effort;
            };

            JointGrpcDataPackController(const std::string &jointName)
                : _name(jointName)
            {}

            virtual void handleDataPackData(const google::protobuf::Message &data) override
            {
                // throws bad_cast
                const auto &j = dynamic_cast<const Gazebo::Joint &>(data);

                std::scoped_lock lock(this->_mutex);

                if(!std::isnan(j.position()))
                    this->_commands.position = j.position();

                if(!std::isnan(j.velocity()))
                    this->_commands.velocity = j.velocity();

                if(!std::isnan(j.effort()))
                    this->_commands.effort = j.effort();
            }

            virtual google::protobuf::Message *getDataPackInformation() override
            {
                auto l = new Gazebo::Joint();

                std::scoped_lock lock(this->_mutex);
                l->set_position(this->_position);
                l->set_velocity(this->_velocity);
                l->set_effort(this->_effort);

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
             * \brief Updates the joint state sent to NRP
             */
            void updateState(double position, double velocity, double effort)
            {
                std::scoped_lock lock(this->_mutex);
                this->_position = position;
                this->_velocity = velocity;
                this->_effort = effort;
            }

        private:

            /*!
             * \brief Joint Name
             */
            std::string _name;

            /*!
             * \brief Protects the joint state and the commands
             */
            std::mutex _mutex;

            /*!
             * \brief Pending commands
             */
            Commands _commands;

            double _position = 0;
            double _velocity = 0;
            double _effort = 0;
    };
}

#endif // GZ_JOINT_GRPC_DATAPACK_CONTROLLER_H

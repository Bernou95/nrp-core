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

#ifndef NRP_JOINT_CONTROLLER_H
#define NRP_JOINT_CONTROLLER_H

#include "nrp_gazebo_grpc_engine/engine_server/joint_set_velocity_datapack/joint_set_velocity_datapack.h"
#include "nrp_general_library/utils/nrp_logger.h"

    class NRPSetJointVelocity
            : public gazebo::ModelPlugin
    {
            struct PIDConfig
                    : public gazebo::common::PID
            {
                enum PID_TYPE { VELOCITY, POSITION };

                PID_TYPE Type = POSITION;

                PIDConfig(double p, double i, double d, PID_TYPE _type);
                PIDConfig(const PIDConfig& pid);

                static PID_TYPE convertStringToType(std::string type);
            };

        public:
            virtual ~NRPSetJointVelocity() override;

            virtual void Load(gazebo::physics::ModelPtr model, sdf::ElementPtr sdf) override;

        private:

            /*!
             * \brief List containing all joint interfaces. TODO: Change to shared_ptr to prevent segfault errors when this plugin is destroyed
             */
            std::list< JointGrpcDataPackController > _jointDataPackControllers;

            /*!
             * \brief Joint PID Configuration
             */

            template<class T>
            static T getOptionalValue(const sdf::ElementPtr &pidConfig, const std::string &key, T defaultValue);
    };

    GZ_REGISTER_MODEL_PLUGIN(NRPSetJointVelocity)

    template<class T>
    T NRPSetJointVelocity::getOptionalValue(const sdf::ElementPtr &pidConfig, const std::string &key, T defaultValue)
    {
        try
        {
            return pidConfig->Get<T>(key);
        }
        catch(const std::exception &e)
        {
            NRPLogger::error(e.what());
        }

        return defaultValue;
    }


#endif

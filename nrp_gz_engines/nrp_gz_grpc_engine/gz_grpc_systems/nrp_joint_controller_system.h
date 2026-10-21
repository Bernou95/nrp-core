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

#ifndef NRP_GZ_JOINT_CONTROLLER_SYSTEM_H
#define NRP_GZ_JOINT_CONTROLLER_SYSTEM_H

#include "nrp_gz_grpc_engine/engine_server/joint_datapack_controller.h"

#include <gz/math/PID.hh>
#include <gz/sim/Joint.hh>
#include <gz/sim/System.hh>

#include <memory>
#include <optional>
#include <vector>

namespace nrp_gz
{
    /*!
     * \brief Model system which registers a datapack for each joint of the model. Optionally, PID controllers
     * can be configured per joint, using the same SDF configuration as the Gazebo Classic NRPJointController plugin:
     *
     * <plugin filename="NRPGzGrpcSystems" name="nrp_gz::NRPJointController">
     *   <joint_name><P>10</P><I>0</I><D>0</D><Type>velocity</Type><Target>0</Target></joint_name>
     * </plugin>
     */
    class NRPJointController
            : public gz::sim::System,
              public gz::sim::ISystemConfigure,
              public gz::sim::ISystemPreUpdate,
              public gz::sim::ISystemPostUpdate,
              public gz::sim::ISystemReset
    {
        public:

            /*!
             * \brief PID controller configuration of a joint
             */
            struct PIDConfig
            {
                enum PID_TYPE { POSITION, VELOCITY };

                gz::math::PID pid;

                PID_TYPE type;

                /*!
                 * \brief Target given in the configuration. If not set, the joint position or velocity
                 * after loading the world is used
                 */
                std::optional<double> initialTarget;

                double target = 0;

                static PID_TYPE convertStringToType(std::string type);
            };

            void Configure(const gz::sim::Entity &entity,
                           const std::shared_ptr<const sdf::Element> &sdf,
                           gz::sim::EntityComponentManager &ecm,
                           gz::sim::EventManager &eventMgr) override;

            void PreUpdate(const gz::sim::UpdateInfo &info, gz::sim::EntityComponentManager &ecm) override;

            void PostUpdate(const gz::sim::UpdateInfo &info, const gz::sim::EntityComponentManager &ecm) override;

            void Reset(const gz::sim::UpdateInfo &info, gz::sim::EntityComponentManager &ecm) override;

        private:

            /*!
             * \brief Data of a single joint
             */
            struct JointData
            {
                std::string name;

                gz::sim::Joint joint;

                std::unique_ptr<JointGrpcDataPackController> controller;

                std::optional<PIDConfig> pid;

                /*!
                 * \brief Effort received from NRP, applied during the next step
                 */
                std::optional<double> pendingEffort;

                double position = 0;
                double velocity = 0;
                double effort = 0;

                /*!
                 * \brief Whether a velocity command was ignored because the joint has no velocity PID
                 */
                bool velocityWarningShown = false;
            };

            std::vector<JointData> _joints;

            /*!
             * \brief Whether PID targets have been initialized with the joint state
             */
            bool _targetsInitialized = false;
    };
}

#endif // NRP_GZ_JOINT_CONTROLLER_SYSTEM_H

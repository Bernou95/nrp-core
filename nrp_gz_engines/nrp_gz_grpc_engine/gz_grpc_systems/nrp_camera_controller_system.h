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

#ifndef NRP_GZ_CAMERA_CONTROLLER_SYSTEM_H
#define NRP_GZ_CAMERA_CONTROLLER_SYSTEM_H

#include "nrp_gz_grpc_engine/engine_server/camera_datapack_controller.h"

#include <gz/msgs/image.pb.h>
#include <gz/sim/System.hh>
#include <gz/transport/Node.hh>

#include <memory>

namespace nrp_gz
{
    /*!
     * \brief Sensor system which registers a datapack for a camera sensor. Must be added to a <sensor type="camera">
     * element. Images are rendered by the gz-sim Sensors system, which must be loaded in the world
     */
    class NRPCameraController
            : public gz::sim::System,
              public gz::sim::ISystemConfigure,
              public gz::sim::ISystemPostUpdate,
              public gz::sim::ISystemReset
    {
        public:
            void Configure(const gz::sim::Entity &entity,
                           const std::shared_ptr<const sdf::Element> &sdf,
                           gz::sim::EntityComponentManager &ecm,
                           gz::sim::EventManager &eventMgr) override;

            void PostUpdate(const gz::sim::UpdateInfo &info, const gz::sim::EntityComponentManager &ecm) override;

            void Reset(const gz::sim::UpdateInfo &info, gz::sim::EntityComponentManager &ecm) override;

        private:

            /*!
             * \brief Callback for new camera images
             */
            void onNewFrame(const gz::msgs::Image &image);

            gz::sim::Entity _sensor = gz::sim::kNullEntity;

            std::unique_ptr<CameraGrpcDataPackController> _cameraInterface;

            gz::transport::Node _node;

            /*!
             * \brief Whether the camera image topic has been subscribed. The topic is only known once the camera
             * sensor has been created by the Sensors system
             */
            bool _subscribed = false;
    };
}

#endif // NRP_GZ_CAMERA_CONTROLLER_SYSTEM_H

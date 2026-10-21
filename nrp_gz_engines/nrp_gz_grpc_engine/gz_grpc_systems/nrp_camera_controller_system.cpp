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

#include "nrp_camera_controller_system.h"

#include "nrp_gz_grpc_engine/engine_server/gz_grpc_engine.h"

// Only one source file of the library may include Register.hh
#include <gz/plugin/RegisterMore.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Sensor.hh>
#include <gz/sim/components/World.hh>

namespace
{
    /*!
     * \brief Scoped name of an entity without the world name, e.g. "model::link"
     */
    std::string scopedNameWithoutWorld(gz::sim::Entity entity, const gz::sim::EntityComponentManager &ecm)
    {
        std::string name;
        while(entity != gz::sim::kNullEntity && !ecm.Component<gz::sim::components::World>(entity))
        {
            const auto *nameComp = ecm.Component<gz::sim::components::Name>(entity);
            const auto entityName = nameComp ? nameComp->Data() : "";
            name = name.empty() ? entityName : entityName + "::" + name;

            entity = ecm.ParentEntity(entity);
        }

        return name;
    }
}

void nrp_gz::NRPCameraController::Configure(const gz::sim::Entity &entity,
                                            const std::shared_ptr<const sdf::Element> &,
                                            gz::sim::EntityComponentManager &ecm,
                                            gz::sim::EventManager &)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    this->_sensor = entity;

    const auto *sensorName = ecm.Component<gz::sim::components::Name>(entity);
    if(!ecm.Component<gz::sim::components::Sensor>(entity) || sensorName == nullptr)
        throw NRPException::logCreate("NRPCameraController must be added to a camera sensor");

    const auto devName = GzGrpcEngine::createDataPackName(scopedNameWithoutWorld(ecm.ParentEntity(entity), ecm), sensorName->Data());
    NRPLogger::info("Registering Camera datapack [ {} ]", devName);

    // Create camera datapack and register it
    this->_cameraInterface.reset(new CameraGrpcDataPackController(devName));
    try {
        GzEngineSingleton::getInstance().engine().registerDataPackWithLock(devName, this->_cameraInterface.get());
    }
    catch(NRPException&) {
        throw NRPException::logCreate("Failed to register Camera datapack. Ensure that this NRP Camera system is "
                                      "used in conjunction with a gz_grpc Engine in an NRP Core experiment.");
    }
}

void nrp_gz::NRPCameraController::PostUpdate(const gz::sim::UpdateInfo &, const gz::sim::EntityComponentManager &ecm)
{
    if(this->_subscribed)
        return;

    // The image topic is set by the Sensors system when the camera sensor is created
    const auto *topic = ecm.Component<gz::sim::components::SensorTopic>(this->_sensor);
    if(topic == nullptr || topic->Data().empty())
        return;

    NRPLogger::debug("Subscribing to camera topic [ {} ]", topic->Data());
    if(!this->_node.Subscribe(topic->Data(), &NRPCameraController::onNewFrame, this))
        throw NRPException::logCreate("Failed to subscribe to camera topic \"" + topic->Data() + "\"");

    this->_subscribed = true;
}

void nrp_gz::NRPCameraController::Reset(const gz::sim::UpdateInfo &, gz::sim::EntityComponentManager &)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    this->_cameraInterface->resetTime();
}

void nrp_gz::NRPCameraController::onNewFrame(const gz::msgs::Image &image)
{
    if(image.width() == 0)
        return;

    const auto &stamp = image.header().stamp();
    const auto sensorTime = std::chrono::seconds(stamp.sec()) + std::chrono::nanoseconds(stamp.nsec());

    this->_cameraInterface->updateCamData(reinterpret_cast<const unsigned char*>(image.data().data()),
                                          image.width(), image.height(), image.step() / image.width(),
                                          std::chrono::duration_cast<std::chrono::steady_clock::duration>(sensorTime));
}

GZ_ADD_PLUGIN(nrp_gz::NRPCameraController,
              gz::sim::System,
              nrp_gz::NRPCameraController::ISystemConfigure,
              nrp_gz::NRPCameraController::ISystemPostUpdate,
              nrp_gz::NRPCameraController::ISystemReset)

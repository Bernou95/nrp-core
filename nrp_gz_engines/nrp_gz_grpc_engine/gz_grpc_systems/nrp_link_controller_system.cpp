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

#include "nrp_link_controller_system.h"

#include "nrp_gz_grpc_engine/engine_server/gz_grpc_engine.h"

// Only one source file of the library may include Register.hh
#include <gz/plugin/RegisterMore.hh>
#include <gz/sim/Model.hh>

void nrp_gz::NRPLinkController::Configure(const gz::sim::Entity &entity,
                                          const std::shared_ptr<const sdf::Element> &,
                                          gz::sim::EntityComponentManager &ecm,
                                          gz::sim::EventManager &)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    gz::sim::Model model(entity);

    try {
        auto &engine = GzEngineSingleton::getInstance().engine();

        // Register a datapack for each link
        for (const auto &linkEntity: model.Links(ecm)) {
            gz::sim::Link link(linkEntity);

            // Velocity components are only filled by the physics system if they exist
            link.EnableVelocityChecks(ecm, true);

            const auto datapackName = GzGrpcEngine::createDataPackName(model.Name(ecm), link.Name(ecm).value_or(""));

            NRPLogger::info("Registering Link datapack [ {} ]", datapackName);

            this->_links.emplace_back(link, std::make_unique<LinkGrpcDataPackController>(datapackName));
            engine.registerDataPackWithLock(datapackName, this->_links.back().second.get());
        }
    }
    catch(NRPException&) {
        throw NRPException::logCreate("Failed to register Link datapack. Ensure that this NRP Link system is "
                                      "used in conjunction with a gz_grpc Engine in an NRP Core experiment.");
    }
}

void nrp_gz::NRPLinkController::PreUpdate(const gz::sim::UpdateInfo &, gz::sim::EntityComponentManager &ecm)
{
    // Velocity components are removed when the world is reset. They are only created if they don't exist
    for(auto &link : this->_links)
        link.first.EnableVelocityChecks(ecm, true);
}

void nrp_gz::NRPLinkController::PostUpdate(const gz::sim::UpdateInfo &, const gz::sim::EntityComponentManager &ecm)
{
    for(auto &[link, controller] : this->_links)
    {
        controller->updateState(link.WorldInertialPose(ecm).value_or(gz::math::Pose3d::Zero),
                                link.WorldLinearVelocity(ecm).value_or(gz::math::Vector3d::Zero),
                                link.WorldAngularVelocity(ecm).value_or(gz::math::Vector3d::Zero));
    }
}

GZ_ADD_PLUGIN(nrp_gz::NRPLinkController,
              gz::sim::System,
              nrp_gz::NRPLinkController::ISystemConfigure,
              nrp_gz::NRPLinkController::ISystemPreUpdate,
              nrp_gz::NRPLinkController::ISystemPostUpdate)

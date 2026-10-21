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

#include "nrp_model_controller_system.h"

#include "nrp_gz_grpc_engine/engine_server/gz_grpc_engine.h"

// Only one source file of the library may include Register.hh
#include <gz/plugin/RegisterMore.hh>
#include <gz/sim/Util.hh>
#include <gz/sim/components/AngularVelocityCmd.hh>
#include <gz/sim/components/LinearVelocityCmd.hh>

void nrp_gz::NRPModelController::Configure(const gz::sim::Entity &entity,
                                           const std::shared_ptr<const sdf::Element> &,
                                           gz::sim::EntityComponentManager &ecm,
                                           gz::sim::EventManager &)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    this->_model = gz::sim::Model(entity);
    this->_canonicalLink = gz::sim::Link(this->_model.CanonicalLink(ecm));

    // Velocity components are only filled by the physics system if they exist
    this->_canonicalLink.EnableVelocityChecks(ecm, true);

    // Register a datapack for the model
    const auto datapackName = this->_model.Name(ecm);
    this->_modelInterface.reset(new ModelGrpcDataPackController(datapackName));
    NRPLogger::info("Registering Model datapack [ {} ]", datapackName);

    try {
        GzEngineSingleton::getInstance().engine().registerDataPackWithLock(datapackName, this->_modelInterface.get());
    }
    catch(NRPException&) {
        throw NRPException::logCreate("Failed to register Model datapack. Ensure that this NRP Model system is "
                                      "used in conjunction with a gz_grpc Engine in an NRP Core experiment.");
    }
}

void nrp_gz::NRPModelController::PreUpdate(const gz::sim::UpdateInfo &, gz::sim::EntityComponentManager &ecm)
{
    const auto entity = this->_model.Entity();

    // Velocity components are removed when the world is reset. They are only created if they don't exist
    this->_canonicalLink.EnableVelocityChecks(ecm, true);

    // Velocity commands set in the previous step have already been applied
    if(this->_velocityCmdSet)
    {
        ecm.RemoveComponent<gz::sim::components::LinearVelocityCmd>(entity);
        ecm.RemoveComponent<gz::sim::components::AngularVelocityCmd>(entity);
        this->_velocityCmdSet = false;
    }

    // Apply commands received from NRP
    const auto commands = this->_modelInterface->takeCommands();

    if(commands.pose)
        this->_model.SetWorldPoseCmd(ecm, commands.pose.value());

    // Velocity commands are expressed in the model frame
    const auto worldRot = commands.pose.value_or(gz::sim::worldPose(entity, ecm)).Rot();

    if(commands.linearVelocity)
    {
        ecm.SetComponentData<gz::sim::components::LinearVelocityCmd>(entity, worldRot.RotateVectorReverse(commands.linearVelocity.value()));
        this->_velocityCmdSet = true;
    }

    if(commands.angularVelocity)
    {
        ecm.SetComponentData<gz::sim::components::AngularVelocityCmd>(entity, worldRot.RotateVectorReverse(commands.angularVelocity.value()));
        this->_velocityCmdSet = true;
    }
}

void nrp_gz::NRPModelController::PostUpdate(const gz::sim::UpdateInfo &, const gz::sim::EntityComponentManager &ecm)
{
    this->_modelInterface->updateState(gz::sim::worldPose(this->_model.Entity(), ecm),
                                       this->_canonicalLink.WorldLinearVelocity(ecm).value_or(gz::math::Vector3d::Zero),
                                       this->_canonicalLink.WorldAngularVelocity(ecm).value_or(gz::math::Vector3d::Zero));
}

GZ_ADD_PLUGIN(nrp_gz::NRPModelController,
              gz::sim::System,
              nrp_gz::NRPModelController::ISystemConfigure,
              nrp_gz::NRPModelController::ISystemPreUpdate,
              nrp_gz::NRPModelController::ISystemPostUpdate)

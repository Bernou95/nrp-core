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

#include "nrp_joint_controller_system.h"

#include "nrp_gz_grpc_engine/engine_server/gz_grpc_engine.h"

#include <gz/plugin/Register.hh>
#include <gz/sim/Model.hh>

#include <algorithm>
#include <map>

namespace
{
    template<class T>
    T getOptionalValue(const sdf::ElementPtr &pluginData, const std::string &key, T defaultValue)
    {
        if(pluginData->HasElement(key))
            return pluginData->Get<T>(key);

        return defaultValue;
    }
}

nrp_gz::NRPJointController::PIDConfig::PID_TYPE nrp_gz::NRPJointController::PIDConfig::convertStringToType(std::string type)
{
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    if(type.compare("position") == 0)
        return POSITION;

    if(type.compare("velocity") == 0)
        return VELOCITY;

    throw std::invalid_argument("No PID of type " + type + " known");
}

void nrp_gz::NRPJointController::Configure(const gz::sim::Entity &entity,
                                           const std::shared_ptr<const sdf::Element> &sdf,
                                           gz::sim::EntityComponentManager &ecm,
                                           gz::sim::EventManager &)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    gz::sim::Model model(entity);
    const auto modelName = model.Name(ecm);

    std::map<std::string, PIDConfig> jointConfigs;

    // Iterate over sdf configurations
    try
    {
        // Read configuration for all joints
        auto pJointPID = sdf->GetFirstElement();
        while(pJointPID != nullptr)
        {
            const auto jointName = pJointPID->GetName();

            // Find corresponding joint
            if(model.JointByName(ecm, jointName) == gz::sim::kNullEntity)
            {
                const auto errMsg = "Joint \"" + jointName + "\" not found in model \"" + modelName + "\"";
                NRPLogger::error(errMsg);
                throw std::logic_error(errMsg);
            }

            // Read PID settings
            PIDConfig jointConfig;
            jointConfig.pid.Init(pJointPID->Get<double>("P"), pJointPID->Get<double>("I"), pJointPID->Get<double>("D"),
                                 getOptionalValue<double>(pJointPID, "IMax", 0), getOptionalValue<double>(pJointPID, "IMin", 0));
            jointConfig.type = PIDConfig::convertStringToType(pJointPID->Get<std::string>("Type"));

            if(pJointPID->HasElement("Target"))
                jointConfig.initialTarget = pJointPID->Get<double>("Target");

            // Save config for later
            jointConfigs.emplace(jointName, jointConfig);

            pJointPID = pJointPID->GetNextElement();
        }
    }
    catch(const std::exception &)
    {
        NRPLogger::error("Error reading configuration for plugin {} of model {}", sdf->Get<std::string>("name"), modelName);
        throw;
    }

    // Initiate datapack interfaces for joints
    const auto joints = model.Joints(ecm);
    this->_joints.reserve(joints.size());

    for(const auto &jointEntity : joints)
    {
        gz::sim::Joint joint(jointEntity);
        const auto jointName = joint.Name(ecm).value_or("");

        // Joint state components are only filled by the physics system if they exist
        joint.EnablePositionCheck(ecm, true);
        joint.EnableVelocityCheck(ecm, true);

        JointData jointData;
        jointData.name = jointName;
        jointData.joint = joint;
        jointData.controller = std::make_unique<JointGrpcDataPackController>(jointName);

        // Check for existing PID Config
        auto pidConfigIterator = jointConfigs.find(jointName);
        if(pidConfigIterator != jointConfigs.end())
            jointData.pid = pidConfigIterator->second;

        // Create datapack
        const auto datapackName = GzGrpcEngine::createDataPackName(modelName, jointName);
        NRPLogger::info("Registering Joint datapack [ {} ]", datapackName);

        try {
            GzEngineSingleton::getInstance().engine().registerDataPackWithLock(datapackName, jointData.controller.get());
        }
        catch(NRPException&) {
            throw NRPException::logCreate("Failed to register Joint datapack. Ensure that this NRP Joint system is "
                                          "used in conjunction with a gz_grpc Engine in an NRP Core experiment.");
        }

        this->_joints.push_back(std::move(jointData));
    }
}

void nrp_gz::NRPJointController::PreUpdate(const gz::sim::UpdateInfo &info, gz::sim::EntityComponentManager &ecm)
{
    for(auto &jointData : this->_joints)
    {
        // Joint state components are removed when the world is reset. They are only created if they don't exist
        jointData.joint.EnablePositionCheck(ecm, true);
        jointData.joint.EnableVelocityCheck(ecm, true);

        // Apply commands received from NRP
        const auto commands = jointData.controller->takeCommands();

        if(commands.position)
            jointData.joint.ResetPosition(ecm, {commands.position.value()});

        if(commands.velocity)
        {
            if(jointData.pid && jointData.pid->type == PIDConfig::VELOCITY)
                jointData.pid->target = commands.velocity.value();
            else if(!jointData.velocityWarningShown)
            {
                NRPLogger::warn("Joint \"{}\" has no velocity PID configured. Velocity commands are ignored", jointData.name);
                jointData.velocityWarningShown = true;
            }
        }

        if(commands.effort)
            jointData.pendingEffort = commands.effort;

        if(info.paused)
            continue;

        // Compute joint force
        double force = 0;
        bool applyForce = false;

        if(jointData.pid)
        {
            const auto &curValue = jointData.pid->type == PIDConfig::POSITION ? jointData.position : jointData.velocity;
            force += jointData.pid->pid.Update(curValue - jointData.pid->target, info.dt);
            applyForce = true;
        }

        if(jointData.pendingEffort)
        {
            force += jointData.pendingEffort.value();
            jointData.pendingEffort.reset();
            applyForce = true;
        }

        if(applyForce)
        {
            jointData.joint.SetForce(ecm, {force});
            jointData.effort = force;
        }
    }
}

void nrp_gz::NRPJointController::PostUpdate(const gz::sim::UpdateInfo &, const gz::sim::EntityComponentManager &ecm)
{
    for(auto &jointData : this->_joints)
    {
        const auto position = jointData.joint.Position(ecm);
        if(position && !position->empty())
            jointData.position = position->front();

        const auto velocity = jointData.joint.Velocity(ecm);
        if(velocity && !velocity->empty())
            jointData.velocity = velocity->front();

        jointData.controller->updateState(jointData.position, jointData.velocity, jointData.effort);

        // Initialize PID targets with the joint state after loading the world
        if(!this->_targetsInitialized && jointData.pid && !jointData.pid->initialTarget)
            jointData.pid->initialTarget = jointData.pid->type == PIDConfig::POSITION ? jointData.position : jointData.velocity;
    }

    if(!this->_targetsInitialized)
    {
        for(auto &jointData : this->_joints)
        {
            if(jointData.pid)
                jointData.pid->target = jointData.pid->initialTarget.value_or(0);
        }

        this->_targetsInitialized = true;
    }
}

void nrp_gz::NRPJointController::Reset(const gz::sim::UpdateInfo &, gz::sim::EntityComponentManager &)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    for(auto &jointData : this->_joints)
    {
        if(jointData.pid)
        {
            jointData.pid->pid.Reset();
            jointData.pid->target = jointData.pid->initialTarget.value_or(0);
        }

        jointData.pendingEffort.reset();
        jointData.effort = 0;
    }
}

GZ_ADD_PLUGIN(nrp_gz::NRPJointController,
              gz::sim::System,
              nrp_gz::NRPJointController::ISystemConfigure,
              nrp_gz::NRPJointController::ISystemPreUpdate,
              nrp_gz::NRPJointController::ISystemPostUpdate,
              nrp_gz::NRPJointController::ISystemReset)

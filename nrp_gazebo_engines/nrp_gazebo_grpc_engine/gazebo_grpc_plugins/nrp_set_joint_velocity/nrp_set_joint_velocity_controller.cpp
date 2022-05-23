//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_set_joint_velocity/nrp_set_joint_velocity_controller.h"
#include "nrp_gazebo_grpc_engine/engine_server/nrp_communication_controller.h"
#include <gazebo/physics/Model.hh>


NRPSetJointVelocity::~NRPSetJointVelocity() = default;

void NRPSetJointVelocity::Load(gazebo::physics::ModelPtr model, sdf::ElementPtr sdf)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

     // The the vector has all the selected links by user typed in the SDF file 
    std::vector<gazebo::physics::JointPtr> selected_joints_by_user;

    // Iterate over sdf configurations
    try
    {
        // Read configuration for all joints
        sdf::ElementPtr sdfReader = sdf->GetFirstElement();
        while(sdfReader != nullptr)
        {
            const auto jointName = sdfReader->GetName();

            std::cout << "_debug *** Joint Names ***: "  << jointName << std::endl;

            // Find corresponding joint data
            gazebo::physics::JointPtr pJoint = model->GetJoint(jointName);
            if(pJoint == nullptr)
            {
                const auto errMsg = "Joint \"" + jointName + "\" not found in model \"" + model->GetScopedName() + "\"";
                NRPLogger::error(errMsg);
                throw std::logic_error(errMsg);
            }
            
            selected_joints_by_user.push_back(pJoint);
            sdfReader = sdfReader->GetNextElement();
        }
    }
    catch(const std::exception &)
    {
        NRPLogger::error("Error reading configuration for plugin {} of model {}", this->GetHandle(), model->GetScopedName());
        throw;
    }

    // Go through only joints selected by user in the sdf file
    for(const gazebo::physics::JointPtr &joint : selected_joints_by_user)
    {

        // _debug for debugging print
        std::cout << "_debug joint->GetName(): " << joint->GetName() << std::endl;

        std::string joint_name = joint->GetName();


        // Create Datapack
        const auto datapackName = NRPCommunicationController::createDataPackName(*this, joint_name);
        NRPLogger::info("Registering joint velocity for joint [ {} ]", joint_name);
        this->_jointDataPackControllers.push_back(JointGrpcDataPackController(joint_name, joint));
        NRPCommunicationController::getInstance().registerDataPack(datapackName, &(this->_jointDataPackControllers.back()));

    }
    // Register plugin
    NRPCommunicationController::getInstance().registerModelPlugin(this);
}

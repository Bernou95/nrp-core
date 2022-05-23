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

#include "nrp_add_relative_force_to_link/nrp_add_link_relative_force.h"
#include "nrp_gazebo_grpc_engine/engine_server/nrp_communication_controller.h"

#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Link.hh>

using namespace nlohmann;

void gazebo::NRPLinkControllerPlugin::Load(gazebo::physics::ModelPtr model, sdf::ElementPtr sdf)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    std::vector<gazebo::physics::LinkPtr> selected_links_by_user;

     try
    {
        // Read written configurations under the plugin in the sdf file
        sdf::ElementPtr sdfReader = sdf->GetFirstElement();
        while(sdfReader != nullptr)
        {   
            //Get written configuration under the plugin line by line
            //Not these are defined by the user in the sdf file.Basically we are reading these written text files
            //There might be typo in these text and therefore it is neccesarry to check and validate them.
            const std::string linkName = sdfReader->GetName();
            // const double jointInitialValue = sdfReader->Get<double>("InitialValue");

            std::cout << "_debug SDF Reader "  << linkName << std::endl;


            // Find corresponding joint data
            gazebo::physics::LinkPtr pLink = model->GetLink(linkName);
            
            if(pLink == nullptr)
                throw NRPException::logCreate("Joint \"" + linkName + "\" not found in model \"" + model->GetScopedName() + "\"");

            selected_links_by_user.push_back(pLink);

            sdfReader = sdfReader->GetNextElement();
        }
    }
    catch(std::exception &e)
    {
        throw NRPException::logCreate(e, "Error reading configuration for plugin \"" + this->GetHandle() + "\" of model \"" + model->GetScopedName() + "\"");
    }


    
    auto &commControl = NRPCommunicationController::getInstance();

    // Register a datapack for each link
    for(const gazebo::physics::LinkPtr &link : selected_links_by_user)
    {
        const auto datapackName = NRPCommunicationController::createDataPackName(*this, link->GetName());

        NRPLogger::info("Registering link controller for link [ {} ]", datapackName);

        this->_linkInterfaces.push_back(ForceGrpcDataPackController(datapackName, link));
        commControl.registerDataPack(datapackName, &(this->_linkInterfaces.back()));
    }

    // Register plugin
    commControl.registerModelPlugin(this);
}

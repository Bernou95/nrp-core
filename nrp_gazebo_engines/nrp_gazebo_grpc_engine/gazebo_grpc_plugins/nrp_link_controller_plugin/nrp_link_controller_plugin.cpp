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

#include "nrp_link_controller_plugin/nrp_link_controller_plugin.h"
<<<<<<< HEAD
#include "nrp_gazebo_grpc_engine/engine_server/nrp_communication_controller.h"
=======

#include "nrp_communication_controller/nrp_communication_controller.h"
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Link.hh>

using namespace nlohmann;

void gazebo::NRPLinkControllerPlugin::Load(gazebo::physics::ModelPtr model, sdf::ElementPtr)
{
<<<<<<< HEAD
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	
=======
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
	auto &commControl = NRPCommunicationController::getInstance();

	// Register a device for each link
	auto links = model->GetLinks();
	for(const auto &link : links)
	{
		const auto deviceName = NRPCommunicationController::createDeviceName(*this, link->GetName());

		std::cout << "Registering link controller for link \"" << deviceName << "\"\n";

<<<<<<< HEAD
		this->_linkInterfaces.push_back(LinkGrpcDeviceController(deviceName, link));
=======
		this->_linkInterfaces.push_back(GrpcDeviceControlSerializer<LinkDeviceController>(deviceName, link));
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
		commControl.registerDevice(deviceName, &(this->_linkInterfaces.back()));
	}
}

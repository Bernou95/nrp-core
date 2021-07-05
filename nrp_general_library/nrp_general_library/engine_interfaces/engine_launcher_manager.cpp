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

#include "nrp_general_library/engine_interfaces/engine_launcher_manager.h"

<<<<<<< HEAD
#include "nrp_general_library/utils/nrp_logger.h"
=======
#include <spdlog/spdlog.h>
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

void EngineLauncherManager::registerLauncher(const EngineLauncherInterfaceSharedPtr &launcher)
{
	const auto &sameLauncherPtr = this->findLauncher(launcher->engineType());
	if(sameLauncherPtr != nullptr)
	{
		// TODO: Handle error where two launchers have same name
<<<<<<< HEAD
		NRPLogger::error("Launcher with same name \"" + sameLauncherPtr->engineType() + "\"already registered");
=======
		spdlog::warn("Launcher with same name \"" + sameLauncherPtr->engineType() + "\"already registered");
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
	}

	this->_launchers.emplace(launcher);
}

EngineLauncherInterfaceSharedPtr EngineLauncherManager::findLauncher(const EngineLauncherInterface::engine_type_t &type) const
{
<<<<<<< HEAD
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	
=======
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
	for(const auto &launcher : this->_launchers)
	{
		if(launcher->engineType() == type)
		{
<<<<<<< HEAD
			NRPLogger::debug("Found launcher for engine " + type);
=======
			spdlog::info("Found launcher for engine " + type);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
			return launcher;
		}
	}

	return EngineLauncherInterfaceSharedPtr(nullptr);
}

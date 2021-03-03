/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020 Michael Zechmair
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

#ifndef OPENSIM_ENGINE_JSON_NRP_CLIENT_H
#define OPENSIM_ENGINE_JSON_NRP_CLIENT_H

#include "nrp_general_library/device_interface/devices/pyobject_device.h"
#include "nrp_json_engine_protocol/nrp_client/engine_json_nrp_client.h"
#include "nrp_general_library/engine_interfaces/engine_interface.h"
#include "nrp_general_library/plugin_system/plugin.h"

#include "nrp_opensim_json_engine/config/opensim_config.h"

#include <unistd.h>

/*!
 * \brief NRP - Python Communicator on the NRP side. Converts DeviceInterface classes from/to JSON objects
 */
class OpensimEngineJSONNRPClient
        : public EngineJSONNRPClient<OpensimEngineJSONNRPClient, OpensimConfig, PyObjectDevice>
{
		/*!
		 * \brief Time (in seconds) to wait for Python to exit cleanly after first SIGTERM signal. Afterwards, send a SIGKILL
		 */
		static constexpr size_t _killWait = 10;

	public:
		OpensimEngineJSONNRPClient(EngineConfigConst::config_storage_t &config, ProcessLauncherInterface::unique_ptr &&launcher);
		virtual ~OpensimEngineJSONNRPClient() override;

		virtual void initialize() override;

		virtual void shutdown() override;

	private:
		/*!
		 * \brief Error message returned by init command
		 */
		std::string _initErrMsg = "";
};

using OpensimEngineJSONLauncher = OpensimEngineJSONNRPClient::EngineLauncher<OpensimConfig::DefEngineType>;


CREATE_NRP_ENGINE_LAUNCHER(OpensimEngineJSONLauncher);


#endif // OPENSIM_ENGINE_JSON_NRP_CLIENT_H

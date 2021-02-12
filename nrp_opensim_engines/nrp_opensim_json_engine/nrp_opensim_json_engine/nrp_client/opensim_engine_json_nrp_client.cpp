//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020 Michael Zechmair
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

#include "nrp_opensim_json_engine/nrp_client/opensim_engine_json_nrp_client.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include <chrono>

OpensimEngineJSONNRPClient::OpensimEngineJSONNRPClient(EngineConfigConst::config_storage_t &config, ProcessLauncherInterface::unique_ptr &&launcher)
    : EngineJSONNRPClient(config, std::move(launcher))
{}

OpensimEngineJSONNRPClient::~OpensimEngineJSONNRPClient()
{}

void OpensimEngineJSONNRPClient::initialize()
{
	const auto &opensimConfig = this->engineConfig();
	nlohmann::json resp = this->sendInitCommand(nlohmann::json({{OpensimConfig::ConfigType.m_data, opensimConfig->writeConfig()}}));
	if(resp.at(OpensimConfig::InitFileExecStatus.data())!=0)
	{
		// Write the error message
		this->_initErrMsg = resp.at(OpensimConfig::InitFileErrorMsg.data());
		NRPLogger::SPDErrLogDefault(this->_initErrMsg);

		throw NRPException::logCreate("Initialization failed: " + this->_initErrMsg);
	}
}

void OpensimEngineJSONNRPClient::shutdown()
{
	this->sendShutdownCommand(nlohmann::json());
}

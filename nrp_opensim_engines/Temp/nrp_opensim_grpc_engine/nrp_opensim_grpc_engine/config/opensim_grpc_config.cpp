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

#include "nrp_opensim_grpc_engine/config/opensim_grpc_config.h"

#include "nrp_opensim_grpc_engine/config/cmake_constants.h"
#include "nrp_general_library/config/cmake_constants.h"

#include <iostream>

const std::vector<std::string> OpensimGrpcConfigConst::DefOpensimPlugins = {};

const size_t OpensimGrpcConfigConst::DefOpensimRNGSeed = static_cast<size_t>(time(nullptr));

OpensimGrpcConfig::OpensimGrpcConfig(EngineConfigConst::config_storage_t &config)
    : EngineGRPCConfig(config,
                       OpensimGrpcConfig::DefOpensimPlugins, OpensimGrpcConfig::DefOpensimRNGSeed,
                       OpensimGrpcConfig::DefMaxOpensimWorldLoadTime)
{}

std::string &OpensimGrpcConfig::opensimWorldFile()
{
	return this->template getPropertyByName<OpensimGrpcConfig::OpensimWorldFileName, std::string>();
}

const std::string &OpensimGrpcConfig::opensimWorldFile() const
{
	return this->template getPropertyByName<OpensimGrpcConfig::OpensimWorldFileName, std::string>();
}

std::vector<std::string> &OpensimGrpcConfig::opensimPlugins()
{
	return this->template getPropertyByName<OpensimGrpcConfig::OpensimPlugins, std::vector<std::string>>();
}

const std::vector<std::string> &OpensimGrpcConfig::opensimPlugins() const
{
	return this->template getPropertyByName<OpensimGrpcConfig::OpensimPlugins, std::vector<std::string>>();
}

size_t &OpensimGrpcConfig::opensimRNGSeed()
{
	return this->template getPropertyByName<OpensimGrpcConfig::OpensimRNGSeed, size_t>();
}

const size_t &OpensimGrpcConfig::opensimRNGSeed() const
{
	return this->template getPropertyByName<OpensimGrpcConfig::OpensimRNGSeed, size_t>();
}

unsigned int &OpensimGrpcConfig::maxOpensimWorldLoadTime()
{
	return this->template getPropertyByName<OpensimGrpcConfig::MaxOpensimWorldLoadTime>();
}

const unsigned int &OpensimGrpcConfig::maxOpensimWorldLoadTime() const
{
	return this->template getPropertyByName<OpensimGrpcConfig::MaxOpensimWorldLoadTime>();
}

EngineConfigConst::string_vector_t OpensimGrpcConfig::allEngineProcEnvParams() const
{
	EngineConfigConst::string_vector_t envVars = this->EngineGRPCConfig::allEngineProcEnvParams();

	// Add NRP and Opensim plugins dir
	envVars.push_back("OPENSIM_PLUGIN_PATH=" NRP_OPENSIM_PLUGINS_DIR ":" DEFAULT_OPENSIM_PLUGIN_DIR ":$OPENSIM_PLUGIN_PATH");

	// Add NRP and Opensim library paths
	envVars.push_back("LD_LIBRARY_PATH=" NRP_LIB_INSTALL_DIR ":" NRP_OPENSIM_PLUGINS_DIR ":$LD_LIBRARY_PATH");

	return envVars;
}

EngineConfigConst::string_vector_t OpensimGrpcConfig::allEngineProcStartParams() const
{
	EngineConfigConst::string_vector_t startParams = this->EngineGRPCConfig::allEngineProcStartParams();

	// Add opensim plugins
	for(const auto &curPlugin : this->opensimPlugins())
	{
		startParams.push_back(OpensimGrpcConfig::OpensimPluginArg.data());
		startParams.push_back(curPlugin);
	}

	// Add opensim communication system plugin
	startParams.push_back(OpensimGrpcConfig::OpensimPluginArg.data());
	startParams.push_back(NRP_OPENSIM_COMMUNICATION_PLUGIN);

	startParams.push_back("--verbose");

	// Add RNG Seed
	startParams.push_back(OpensimGrpcConfig::OpensimRNGSeedArg.data());
	startParams.push_back(std::to_string(this->opensimRNGSeed()));

	// Add world file
	startParams.push_back(this->opensimWorldFile());

	return startParams;
}

bool OpensimGrpcConfig::checkEnvVariableFormat(const std::string &envVariable)
{
	const auto eqPos = envVariable.find_first_of('=');

	// Check that an '=' is present, and neither the first nor last element in the sting
	if(eqPos == 0 || eqPos >= envVariable.length()-1)
		return false;
	return true;
}

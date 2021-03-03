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

#include "nrp_new_gazebo_engine/config/new_gazebo_config.h"

#include "nrp_general_library/config/cmake_constants.h"

NewGazeboConfig::NewGazeboConfig(EngineConfigConst::config_storage_t &config)
    : NewGazeboConfig(config.Data){
	config.Config = this;
}

NewGazeboConfig::NewGazeboConfig(const nlohmann::json &data)
	: EngineJSONConfig(data,
		NewGazeboConfig::DefNewGazeboRunPy.data(),
		NewGazeboConfig::DefNewGazeboFileName.data()){}

std::string &NewGazeboConfig::newGazeboRunPy(){
	return this->getPropertyByName<NewGazeboConfig::NewGazeboRunPy, std::string>();
}
const std::string &NewGazeboConfig::newGazeboRunPy() const{
	return this->getPropertyByName<NewGazeboConfig::NewGazeboRunPy, std::string>();
}

std::string &NewGazeboConfig::newGazeboFileName(){
	return this->getPropertyByName<NewGazeboConfig::NewGazeboFileName, std::string>();
}
const std::string &NewGazeboConfig::newGazeboFileName() const{
	return this->getPropertyByName<NewGazeboConfig::NewGazeboFileName, std::string>();
}


EngineConfigConst::string_vector_t NewGazeboConfig::allEngineProcStartParams() const{
	EngineConfigConst::string_vector_t startParams = this->EngineJSONConfig::allEngineProcStartParams();

	// Add JSON Server address (will be used by plugin)
	startParams.push_back(std::string("--") + NewGazeboConfig::EngineServerAddrArg.data() + "=" + this->engineServerAddress());

	return startParams;
}

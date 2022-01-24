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

#include "nrp_docker_sim_engine/nrp_client/docker_sim_engine.h"
#include "Python.h"

namespace{
    bool isDataPackTypeValid(const DataPackIdentifier & datapack, const std::string & engineName)
    {
        // Check if type matches the NestServerDataPack type
        // Skip the check if type was not set
        const bool isTypeValid = (datapack.Type.empty() || datapack.Type == JsonDataPack::getType());
        return (isTypeValid && datapack.EngineName == engineName);
    }
}
SimulationTime DockerSimEngine::runLoopStepCallback(SimulationTime timeStep){
    std::string header = "http://";
    std::string tail = "//transfer";
    std::string addr = this->engineConfig()["ServerAddress"];
    std::string targetWeb = header+addr+tail;
    
    auto infoMsg = this->sendRequest(targetWeb, "application/json", this->pCommond,
            "Engine server \"" + this->engineName() + "\" failed during Network");
    this->pInfo = infoMsg.dump();
    return timeStep;
}

EngineClientInterface::datapacks_set_t DockerSimEngine::getDataPacksFromEngine(const datapack_identifiers_set_t &datapackIdentifiers)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    EngineClientInterface::datapacks_set_t retVals;

    for(const auto &devID : datapackIdentifiers)
    {   
        if(isDataPackTypeValid(devID, this->engineName()))
        {
            retVals.emplace(new JsonDataPack(devID.Name, devID.EngineName,
                new nlohmann::json(nlohmann::json::parse(this->pInfo))));
        }
    }

    return retVals;
}

void DockerSimEngine::sendDataPacksToEngine(const datapacks_ptr_t &datapacksArray)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    for(DataPackInterface * const datapack : datapacksArray)
    {
        if(isDataPackTypeValid(datapack->id(), this->engineName()))
        {
            // Send command along with parameters to Docker simulator
            this->pCommond = ((JsonDataPack const *)datapack)->getData().dump();
        }
    }
}
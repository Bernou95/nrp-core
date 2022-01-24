
/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
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

#ifndef DOCKER_SIM_ENGINE_H
#define DOCKER_SIM_ENGINE_H

//#include "nrp_docker_sim_engine/nrp_client/docker_sim_nrp_client.h"
#include "nrp_general_library/plugin_system/plugin.h"

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_json_engine_protocol/nrp_client/engine_json_nrp_client.h"
#include "nrp_json_engine_protocol/config/engine_json_config.h"

#include "nrp_docker_sim_engine/config/docker_config.h"

/*!
 * \brief PythonJSONEngine client
 */
class DockerSimEngine
: public EngineJSONNRPClient<DockerSimEngine, DockerConfigConst::EngineSchema>
{
public:

    DockerSimEngine(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
            : EngineJSONNRPClient(config, std::move(launcher))
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);
        setDefaultProperty<std::string>("EngineProcCmd", "NONE");
    }

    virtual void initialize() override
    {   NRP_LOGGER_TRACE("{} called", __FUNCTION__);    }
    virtual void reset() override
    {   NRP_LOGGER_TRACE("{} called", __FUNCTION__); std::cout << "reset\n"; }
    virtual void shutdown() override
    {   std::cout << "Docker Engine is shutting down !!!\n"; }
    

    virtual const std::vector<std::string> engineProcStartParams() const override
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);
        std::vector<std::string> startParams;
        // Add Server address
        startParams.push_back("--host");
        startParams.push_back(this->engineConfig().at("IPHost"));
        return startParams;
    }
    
    SimulationTime runLoopStepCallback(SimulationTime timeStep) override;
    virtual void sendDataPacksToEngine(const datapacks_ptr_t &datapacksArray) override;
    virtual datapacks_set_t getDataPacksFromEngine(const datapack_identifiers_set_t &datapackIdentifiers) override;

    std::string pCommond = "None";
    std::string pInfo = "{\"nodes\":\"None\"}";
};

using DockerSimEngineLauncher = DockerSimEngine::EngineLauncher<DockerConfigConst::EngineType>;

CREATE_NRP_ENGINE_LAUNCHER(DockerSimEngineLauncher);

#endif // DOCKER_SIM_ENGINE_H

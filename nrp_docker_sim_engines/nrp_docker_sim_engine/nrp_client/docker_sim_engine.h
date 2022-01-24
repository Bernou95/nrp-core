
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

#include "nrp_docker_sim_engine/nrp_client/docker_sim_nrp_client.h"
#include "nrp_general_library/plugin_system/plugin.h"


/*!
 * \brief PythonJSONEngine client
 */
class DockerSimEngine
: public DockerSimNrpClient<DockerSimEngine, DockerConfigConst::EngineSchema>
{
public:

    DockerSimEngine(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
            : DockerSimNrpClient(config, std::move(launcher))
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);
        setDefaultProperty<std::string>("EngineProcCmd", "NONE");
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

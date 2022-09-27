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

#ifndef ${engine_name_uppercase}_JSON_CLIENT_H
#define ${engine_name_uppercase}_JSON_CLIENT_H

#include "nrp_json_engine_protocol/nrp_client/engine_json_nrp_client.h"
#include "${engine_name_lowercase}_json_engine/config/${engine_name_lowercase}_config.h"
#include "nrp_general_library/plugin_system/plugin.h"

class ${engine_name}EngineJSONClient
    : public EngineJSONNRPClient<${engine_name}EngineJSONClient, ${engine_name}JSONConfigConst::EngineSchema>
{
    public:
        ${engine_name}EngineJSONClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher);
        ~${engine_name}EngineJSONClient() override = default;

        /*!
         * \brief Initializes the engine
         *
         * The function will be called at the start of the simulation. It should
         * execute RPC / REST calls that will prepare the engine server and simulator
         * to run the simulation.
         */
        void initialize(const nlohmann::json &) override;

        /*!
         * \brief Shutdowns the engine
         *
         * The function will be called at the end of the simulation. It should
         * execute RPC / REST calls that will shutdown the simulator and engine
         * server gracefully.
         */
        void shutdown() override;

        /*!
         * \brief Resets the engine
         *
         * The function should execute RPC / REST calls that will reset the simulation
         * to its initial state.
         */
        void reset(const nlohmann::json &) override;
};

using ${engine_name}EngineJSONLauncher = ${engine_name}EngineJSONClient::EngineLauncher<${engine_name}JSONConfigConst::EngineType>;

CREATE_NRP_ENGINE_LAUNCHER(${engine_name}EngineJSONLauncher);

#endif // ${engine_name_uppercase}_JSON_CLIENT_H

// EOF
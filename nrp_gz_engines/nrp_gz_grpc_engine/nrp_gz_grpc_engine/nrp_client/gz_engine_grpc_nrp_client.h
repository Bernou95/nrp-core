/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef GZ_ENGINE_GRPC_NRP_CLIENT_H
#define GZ_ENGINE_GRPC_NRP_CLIENT_H

#include "nrp_grpc_engine_protocol/engine_client/engine_grpc_client.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_general_library/plugin_system/plugin.h"

#include "nrp_protobuf/gazebo.pb.h"

#include "nrp_gz_grpc_engine/config/gz_grpc_config.h"

/*!
 *  \brief NRP - Gazebo Harmonic (gz-sim) Communicator on the NRP side. Converts DataPackInterface classes from/to protobuf messages
 */
class GzEngineGrpcNRPClient
: public EngineGrpcClient<GzEngineGrpcNRPClient, GzGrpcConfigConst::EngineSchema>
{
    public:
        GzEngineGrpcNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher);
        virtual ~GzEngineGrpcNRPClient() override = default;

        virtual void initialize() override;

        virtual void reset() override;

        virtual void shutdown() override;
};

using GzEngineGrpcLauncher = GzEngineGrpcNRPClient::EngineLauncher<GzGrpcConfigConst::EngineType>;


CREATE_NRP_ENGINE_LAUNCHER(GzEngineGrpcLauncher);

#endif // GZ_ENGINE_GRPC_NRP_CLIENT_H

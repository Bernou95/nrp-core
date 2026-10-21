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

#include "nrp_gz_grpc_engine/nrp_client/gz_engine_grpc_nrp_client.h"

#include "nrp_gz_grpc_engine/config/cmake_constants.h"

GzEngineGrpcNRPClient::GzEngineGrpcNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
    : EngineGrpcClient(config, std::move(launcher))
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    setDefaultProperty<std::string>("EngineProcCmd", NRP_GZ_GRPC_ENGINE_EXECUTABLE);
    setDefaultProperty<std::vector<std::string>>("GazeboPlugins", std::vector<std::string>());

    // gz-sim systems need a class name besides the library, so they can't be given as a plain list of plugin files
    if(!this->engineConfig().at("GazeboPlugins").empty())
        NRPLogger::warn("Engine \"{}\": \"GazeboPlugins\" is not supported by the gz_grpc engine and will be ignored. "
                        "Add the systems to the world SDF file instead.", this->engineName());
}

void GzEngineGrpcNRPClient::initialize()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Wait for gz-sim to load world
    try
    {
        this->sendInitializeCommand(this->engineConfig());
    }
    catch(std::exception& e)
    {
        throw NRPException::logCreate(e, "Engine \"" + this->engineName() + "\" initialization failed");
    }

    NRPLogger::debug("GzEngineGrpcNRPClient::initialize(...) completed with no errors.");
}

void GzEngineGrpcNRPClient::reset()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    try
    {
        this->sendResetCommand();
    }
    catch(std::exception& e)
    {
        throw NRPException::logCreate(e, "Engine \"" + this->engineName() + "\" reset failed");
    }

    this->resetEngineTime();
}

void GzEngineGrpcNRPClient::shutdown()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    try
    {
        this->sendShutdownCommand(nlohmann::json());
    }
    catch(std::exception &e)
    {
        throw NRPException::logCreate("Engine \"" + this->engineName() + "\" shutdown failed");
    }
}

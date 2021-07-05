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

#include <gtest/gtest.h>
<<<<<<< HEAD
#include <string>
#include <vector>

#include "nrp_general_library/process_launchers/process_launcher_basic.h"
#include "nrp_general_library/process_launchers/launch_commands/launch_command.h"
=======

#include "nrp_general_library/process_launchers/process_launcher_basic.h"
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
#include "nrp_general_library/utils/pipe_communication.h"
#include "nrp_general_library/utils/json_schema_utils.h"

#include "tests/test_env_cmake.h"

TEST(ProcessLauncherBasicTest, TestLaunch)
{
	ProcessLauncherBasic launcher;
	PipeCommunication pCommPtC;
	PipeCommunication pCommCtP;

	// Add test params and envs
	std::vector<std::string> startParams;
	startParams.push_back(std::to_string(pCommPtC.readFd()));
	startParams.push_back(std::to_string(pCommCtP.writeFd()));

	std::vector<std::string> envVars;
	envVars.push_back(TEST_PROC_ENV_VAR_NAME "=" TEST_PROC_ENV_VAR_VAL);

	nlohmann::json config = R"({"EngineName" : "test_engine", "EngineType" : "test_engine"})"_json;
	json_utils::validate_json(config, "https://neurorobotics.net/engines/engine_base.json#EngineBase");

	config["EngineProcCmd"] = TEST_NRP_PROCESS_EXEC;


	// Fork engine process
	ASSERT_GE(launcher.launchEngineProcess(config, envVars, startParams), 0);

	pCommCtP.closeWrite();
	pCommPtC.closeRead();

	// Sync processes
	char readDat[50] = "";
	pCommCtP.readP(readDat, sizeof(TEST_PROC_STR_START), 5, 1);
	ASSERT_STREQ(readDat, TEST_PROC_STR_START);

	pCommPtC.writeP(TEST_PROC_STR_START, sizeof(TEST_PROC_STR_START), 5, 1);

	// Test that env value was set properly
	ASSERT_EQ(pCommCtP.readP(readDat, sizeof(TEST_PROC_ENV_VAR_VAL), 5, 1), sizeof(TEST_PROC_ENV_VAR_VAL));
	ASSERT_STREQ(readDat, TEST_PROC_ENV_VAR_VAL);

	// Stop engine
	ASSERT_LE(launcher.stopEngineProcess(5), 0);

	ASSERT_EQ(pCommCtP.readP(readDat, sizeof(TEST_PROC_STR_SIGTERM), 5, 1), sizeof(TEST_PROC_STR_SIGTERM));
	ASSERT_STREQ(readDat, TEST_PROC_STR_SIGTERM);
}
<<<<<<< HEAD


TEST(ProcessLauncherBasicTest, TestEmptyLaunchCommand)
{

	ProcessLauncherBasic launcher;

	nlohmann::json config = R"({"EngineName" : "test_engine",
								"EngineType" : "test_engine",
								"EngineLaunchCommand": "EmptyLaunchCommand"})"_json;

	json_utils::validate_json(config, "https://neurorobotics.net/engines/engine_base.json#EngineBase");

	std::vector<std::string> emptyVector;
	auto & envVars = emptyVector;
	auto & startParams = emptyVector;

	// the launching of an Engine with a Empty launch command returns -1
	ASSERT_GE(launcher.launchEngineProcess(config, startParams, envVars), -1);

	// the RUNNING_STATUS an Empty launch command process is always UNKNOWN
	ASSERT_EQ(launcher.getProcessStatus(), LaunchCommandInterface::ENGINE_RUNNING_STATUS::UNKNOWN);

	// stop must return 0
	ASSERT_EQ(launcher.stopEngineProcess(42), 0);

}
=======
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

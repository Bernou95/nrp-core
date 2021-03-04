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

#include <gtest/gtest.h>


#include <boost/python.hpp>
#include <future>
#include <restclient-cpp/restclient.h>

#include "nrp_general_library/device_interface/devices/pyobject_device.h"
#include "nrp_general_library/utils/python_interpreter_state.h"

#include "tests/test_env_cmake.h"
#include "nrp_opensim_json_engine/engine_server/opensim_json_server.h"

namespace python = boost::python;

TEST(TestOpensimJSONServer, TestInitError)
{
	std::string argvDat = "TestProg";
	char *argv = &argvDat[0];
	PythonInterpreterState pyState(1, &argv);

	auto pyGlobals = python::dict(python::import("__main__").attr("__dict__"));
	python::object pyLocals;

	auto cfg = OpensimConfig(nlohmann::json());
	cfg.opensimRunPy() = TEST_OPENSIM_INIT_ERROR_FILE_NAME;
	cfg.opensimFileName() = TEST_OPENSIM_MODEL_NAME;
	cfg.engineServerAddress() = "localhost:1234";

	OpensimJSONServer server(cfg.engineServerAddress(), pyGlobals, pyLocals);

	nlohmann::json req = nlohmann::json({{OpensimConfig::ConfigType.m_data, cfg.writeConfig()}});
	pyState.allowThreads();

	EngineJSONServer::mutex_t fakeMutex;
	EngineJSONServer::lock_t fakeLock(fakeMutex);
	nlohmann::json respParse = server.initialize(req, fakeLock);

	pyState.endAllowThreads();
}

TEST(TestOpensimJSONServer, TestFunc)
{
	std::string argvDat = "TestProg";
	char *argv = &argvDat[0];
	PythonInterpreterState pyState(1, &argv);

	python::dict pyGlobals = python::dict(python::import("__main__").attr("__dict__"));
	python::object pyLocals;

	auto cfg = OpensimConfig(nlohmann::json());
	cfg.opensimRunPy() = TEST_SIMPLE_OPENSIM_FILE_NAME;
	cfg.opensimFileName() = TEST_OPENSIM_MODEL_NAME;
	cfg.engineServerAddress() = "localhost:2345";

	OpensimJSONServer server(cfg.engineServerAddress(), pyGlobals, pyLocals);

	// Test Init
	nlohmann::json req = nlohmann::json({{OpensimConfig::ConfigType.m_data, cfg.writeConfig()}});
	pyState.allowThreads();

	EngineJSONServer::mutex_t fakeMutex;
	EngineJSONServer::lock_t fakeLock(fakeMutex);
	nlohmann::json respParse = server.initialize(req, fakeLock);

	const auto execResult = respParse[OpensimConfig::InitFileExecStatus.data()].get<bool>();
	ASSERT_EQ(execResult, true);
	ASSERT_EQ(server.initRunFlag(), true);

	// Test runStep REST call
	const SimulationTime timeStep(1000);
	ASSERT_EQ(server.runLoopStep(timeStep), timeStep);

	// Test getDevice REST call EngineServerGetDevicesRoute
	server.startServerAsync();
	//pyState.endAllowThreads();
	req = nlohmann::json({{"device1", 0}});
	auto resp = RestClient::post(cfg.engineServerAddress() + "/" + EngineJSONConfigConst::EngineServerGetDevicesRoute.data(), EngineJSONConfigConst::EngineServerContentType.data(), req.dump());
	respParse = nlohmann::json::parse(resp.body);

	pyState.endAllowThreads();
	server.shutdownServer();
}

TEST(TestOpensimJSONServer, TestDevice)
{
	std::string argvDat = "TestProg";
	char *argv = &argvDat[0];
	PythonInterpreterState pyState(1, &argv);

	python::dict pyGlobals = python::dict(python::import("__main__").attr("__dict__"));
	python::object pyLocals;

	auto cfg = OpensimConfig(nlohmann::json());
	cfg.opensimRunPy() = TEST_OPENSIM_EXE_FILE_NAME;
	cfg.opensimFileName() = TEST_OPENSIM_MODEL_NAME;
	cfg.engineServerAddress() = "localhost:3456";

	OpensimJSONServer server(cfg.engineServerAddress(), pyGlobals, pyLocals);

	// Test Init
	nlohmann::json req = nlohmann::json({{OpensimConfig::ConfigType.m_data, cfg.writeConfig()}});
	pyState.allowThreads();

	EngineJSONServer::mutex_t fakeMutex;
	EngineJSONServer::lock_t fakeLock(fakeMutex);
	nlohmann::json respParse = server.initialize(req, fakeLock);

	const auto execResult = respParse[OpensimConfig::InitFileExecStatus.data()].get<bool>();
	ASSERT_EQ(execResult, true);
	ASSERT_EQ(server.initRunFlag(), true);

	// Test runStep REST call
	const SimulationTime timeStep(1000);
	ASSERT_EQ(server.runLoopStep(timeStep), timeStep);

	// Test getDevice REST call EngineServerGetDevicesRoute
	server.startServerAsync();

	//pyState.endAllowThreads();
	req = nlohmann::json({{"device1", 0}});
	auto resp = RestClient::post(cfg.engineServerAddress() + "/" + EngineJSONConfigConst::EngineServerGetDevicesRoute.data(), EngineJSONConfigConst::EngineServerContentType.data(), req.dump());
	respParse = nlohmann::json::parse(resp.body);

	// TODO Why return here?
	return;
	// Test Python Device data deserialization
	PyObjectDevice dev = JSONDeviceConversionMechanism<>::deserialize<PyObjectDevice>(respParse.begin());
	dev.PyObjectDevice::data() = python::dict(dev.PyObjectDevice::data().deserialize(""));
	// TODO: Test Sending data
	ASSERT_EQ(respParse["device1"][PyObjectDeviceConst::Object.m_data].size(), python::len(dev.data()));
	float recTime = python::extract<float>(dev.data()["time_1"]);
	ASSERT_EQ(0, recTime);

	pyState.endAllowThreads();
	server.shutdownServer();
}
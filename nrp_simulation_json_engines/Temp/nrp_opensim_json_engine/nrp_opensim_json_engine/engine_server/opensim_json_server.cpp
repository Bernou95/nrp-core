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

#include "nrp_opensim_json_engine/engine_server/opensim_json_server.h"

#include "nrp_general_library/utils/python_error_handler.h"
#include "nrp_general_library/utils/python_interpreter_state.h"

#include "nrp_opensim_json_engine/config/cmake_constants.h"
#include "nrp_opensim_json_engine/devices/opensim_engine_json_device_controller.h"
#include "nrp_opensim_json_engine/python/opensim_engine_script.h"

#include <fstream>

namespace python = boost::python;

OpensimJSONServer *OpensimJSONServer::_registrationOpensimServer = nullptr;

OpensimJSONServer::OpensimJSONServer(const std::string &serverAddress, python::dict globals, python::object locals)
    : EngineJSONServer(serverAddress),
      _opensimGlobals(globals),
      _opensimLocals(locals)
{}

OpensimJSONServer::OpensimJSONServer(const std::string &serverAddress, const std::string &engineName, const std::string &registrationAddress, python::dict globals, boost::python::object locals)
    : EngineJSONServer(serverAddress, engineName, registrationAddress),
      _opensimGlobals(globals),
      _opensimLocals(locals)
{}

bool OpensimJSONServer::initRunFlag() const
{
	return this->_initRunFlag;
}

bool OpensimJSONServer::shutdownFlag() const
{
	return this->_shutdownFlag;
}

SimulationTime OpensimJSONServer::runLoopStep(SimulationTime timestep)
{
	PythonGILLock lock(this->_opensimGILState, true);

	try
	{
		OpensimEngineScript &script = python::extract<OpensimEngineScript&>(this->_opensimEngineScript);
		return script.runLoop(timestep);
	}
	catch(python::error_already_set &)
	{
		// If an error occured, return the message to the NRP server
		throw NRPExceptionNonRecoverable(handle_pyerror());
	}
}

nlohmann::json OpensimJSONServer::initialize(const nlohmann::json &data, EngineJSONServer::lock_t&)
{
	PythonGILLock lock(this->_opensimGILState, true);
	try
	{
		// Load python
		this->_opensimGlobals.update(python::dict(python::import(NRP_OPENSIM_ENGINE_MODULE_STR).attr("__dict__")));
	}
	catch(python::error_already_set &)
	{
		// If an error occured, return the message to the NRP server without setting the initRunFlag
		return this->formatInitErrorMessage(handle_pyerror());
	}

	// Read received configuration
	const OpensimConfig config(data.at(OpensimConfig::ConfigType.m_data));

	// Read python script file if present
	const std::filesystem::path fileName = config.opensimFileName();
	if(fileName.empty())
	{
		const auto errMsg = "No opensim filename given. Aborting...";
		NRPLogger::SPDErrLogDefault(errMsg);
		return this->formatInitErrorMessage(errMsg);
	}

	if(!std::filesystem::exists(fileName))
	{
		const auto errMsg = "Could not find init file " + std::string(fileName);
		NRPLogger::SPDErrLogDefault(errMsg);
		return this->formatInitErrorMessage(errMsg);
	}

	// Prepare registration
	OpensimJSONServer::_registrationOpensimServer = this;

	// Read python file
	try
	{
		python::exec_file(fileName.c_str(), this->_opensimGlobals, this->_opensimLocals);
	}
	catch(python::error_already_set &)
	{
		// If an error occured, return the message to the NRP server without setting the initRunFlag
		const auto msg = handle_pyerror();
		NRPLogger::SPDErrLogDefault(msg);
		return this->formatInitErrorMessage(msg);
	}

	// Check that executed file also
	if(OpensimJSONServer::_registrationOpensimServer != nullptr)
	{
		OpensimJSONServer::_registrationOpensimServer = nullptr;
		const auto errMsg = "Failed to initialize OpenSim server. Given OpenSim file \"" + std::string(fileName) + "\" does not register a script";
		NRPLogger::SPDErrLogDefault(errMsg);
		return this->formatInitErrorMessage(errMsg);
	}

	// Run user-defined initialize function
	try
	{
		OpensimEngineScript &script = python::extract<OpensimEngineScript&>(this->_opensimEngineScript);
		script.initialize();
	}
	catch(python::error_already_set &)
	{
		// If an error occured, return the message to the NRP server without setting the initRunFlag
		const auto msg = handle_pyerror();
		NRPLogger::SPDErrLogDefault(msg);
		return this->formatInitErrorMessage(msg);

	}

	// Init has run once
	this->_initRunFlag = true;

	// Return success and parsed devmap
	return nlohmann::json({{OpensimConfig::InitFileExecStatus, true}});
}

nlohmann::json OpensimJSONServer::shutdown(const nlohmann::json &)
{
	PythonGILLock lock(this->_opensimGILState, true);

	this->_shutdownFlag = true;

	if(this->_initRunFlag)
	{
		// Run user-defined Shutdown fcn
		try
		{
			OpensimEngineScript &script = python::extract<OpensimEngineScript&>(this->_opensimEngineScript);
			script.shutdown();
		}
		catch(python::error_already_set &)
		{
			// If an error occured, return the message to the NRP server
			throw NRPExceptionNonRecoverable(handle_pyerror());
		}
	}

	// Remove device controllers
	this->clearRegisteredDevices();
	this->_deviceControllerPtrs.clear();

	return nlohmann::json();
}

OpensimEngineScript *OpensimJSONServer::registerScript(const boost::python::object &opensimScript)
{
	assert(OpensimJSONServer::_registrationOpensimServer != nullptr);

	// Register script with server
	OpensimJSONServer::_registrationOpensimServer->_opensimEngineScript = opensimScript();

	// Register server with script
	OpensimEngineScript &script = boost::python::extract<OpensimEngineScript&>(OpensimJSONServer::_registrationOpensimServer->_opensimEngineScript);
	script.setOpensimJSONServer(OpensimJSONServer::_registrationOpensimServer);

	OpensimJSONServer::_registrationOpensimServer = nullptr;

	return &script;
}

nlohmann::json OpensimJSONServer::formatInitErrorMessage(const std::string &errMsg)
{
	return nlohmann::json({{OpensimConfig::InitFileExecStatus, 0}, {OpensimConfig::InitFileErrorMsg, errMsg}});
}

nlohmann::json OpensimJSONServer::getDeviceData(const nlohmann::json &reqData)
{
	PythonGILLock lock(this->_opensimGILState, true);
	return this->EngineJSONServer::getDeviceData(reqData);
}

nlohmann::json OpensimJSONServer::setDeviceData(const nlohmann::json &reqData)
{
	PythonGILLock lock(this->_opensimGILState, true);
	return this->EngineJSONServer::setDeviceData(reqData);
}

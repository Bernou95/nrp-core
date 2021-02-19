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

<<<<<<< HEAD
#include "nrp_general_library/utils/nrp_exceptions.h"

#include "nrp_opensim_json_engine/python/py_engine_script.h"
#include "nrp_opensim_json_engine/devices/opensim_engine_json_device_controller.h"
=======
#include "nrp_opensim_json_engine/python/py_engine_script.h"

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_opensim_json_engine/engine_server/opensim_engine_json_device_controller.h"
#include <iostream>
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37

PyEngineScript::~PyEngineScript()
{
	this->_pServer = nullptr;
}

void PyEngineScript::initialize()
{}

void PyEngineScript::shutdown()
{}

SimulationTime PyEngineScript::simTime() const
{	return this->_time;	}
<<<<<<< HEAD
std::string PyEngineScript::worldFile() const{
	return this->_worldFileName;
}

=======
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37

void PyEngineScript::registerDevice(std::string deviceName)
{
	//std::cout << "Registering device \"" + deviceName + "\"\n";
	assert(this->_pServer != nullptr);

	//std::cout << "Creating device controller for \"" + deviceName + "\"\n";
	PtrTemplates<OpensimEngineJSONDeviceController<PyObjectDevice>>::shared_ptr
	        newController(new OpensimEngineJSONDeviceController<PyObjectDevice>(DeviceIdentifier(deviceName, "", PyObjectDevice::TypeName.data())));

	//std::cout << "Adding device controller for \"" + deviceName + "\"\n";
	this->_deviceControllers.push_back(newController);
	this->_nameDeviceMap.emplace(deviceName, &(newController->data()));

	//std::cout << "Adding device \"" + deviceName + "\" to server\n";
	this->_pServer->registerDeviceNoLock(deviceName, newController.get());

	//std::cout << "Finished registering device \"" + deviceName + "\"\n";
}

boost::python::object &PyEngineScript::getDevice(const std::string &deviceName)
{
	auto devIt = this->_nameDeviceMap.find(deviceName);
<<<<<<< HEAD
=======

>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37
	if(devIt == this->_nameDeviceMap.end())
		throw NRPException::logCreate("Could not find device with name \"" + deviceName + "\"");

	return *(devIt->second);
}

void PyEngineScript::setDevice(const std::string &deviceName, boost::python::object data)
<<<<<<< HEAD
{	this->getDevice(deviceName) = data;	}
=======
{	
	//std::cout << this->_time << std::endl;
	this->getDevice(deviceName) = data;	
}
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37

void PyEngineScript::setOpensimJSONServer(OpensimJSONServer *pServer)
{
	this->_pServer = pServer;
<<<<<<< HEAD
	this->_worldFileName = pServer->getWorldFile();
	std::cout << "From PyEngineScript :--> " << pServer->getWorldFile() << std::endl;
=======
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37
}

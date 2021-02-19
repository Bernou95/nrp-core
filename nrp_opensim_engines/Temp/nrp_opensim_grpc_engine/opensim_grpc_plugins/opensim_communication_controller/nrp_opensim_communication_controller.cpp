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

#include "opensim_communication_controller/nrp_opensim_communication_controller.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

std::unique_ptr<NRPOpensimCommunicationController> NRPOpensimCommunicationController::_instance = nullptr;

NRPOpensimCommunicationController::~NRPOpensimCommunicationController()
{
	this->_stepController = nullptr;
}

NRPOpensimCommunicationController &NRPOpensimCommunicationController::getInstance()
{
	return *(NRPOpensimCommunicationController::_instance.get());
}

NRPOpensimCommunicationController &NRPOpensimCommunicationController::resetInstance(const std::string &serverURL)
{
	// Remove old server, start new one with given server URL
	NRPOpensimCommunicationController::_instance.reset(new NRPOpensimCommunicationController(serverURL));
	return NRPOpensimCommunicationController::getInstance();
}

NRPOpensimCommunicationController &NRPOpensimCommunicationController::resetInstance(const std::string &serverURL, const std::string &engineName, const std::string &registrationURL)
{
	NRPOpensimCommunicationController::_instance.reset(new NRPOpensimCommunicationController(serverURL, engineName, registrationURL));
	return NRPOpensimCommunicationController::getInstance();
}

void NRPOpensimCommunicationController::registerStepController(OpensimStepController *stepController)
{
	EngineGrpcServer::lock_t lock(this->_deviceLock);
	this->_stepController = stepController;
}

SimulationTime NRPOpensimCommunicationController::runLoopStep(SimulationTime timeStep)
{
	if(this->_stepController == nullptr)
	{
		auto err = std::out_of_range("Tried to run loop while the controller has not yet been initialized");
		std::cerr << err.what();

		throw err;
	}

	try
	{
		// Execute loop step (Note: The _deviceLock mutex has already been set by EngineJSONServer::runLoopStepHandler, so no calls to reading/writing from/to devices is possible at this moment)
		return this->_stepController->runLoopStep(timeStep);
	}
	catch(const std::exception &e)
	{
		std::cerr << "Error during Opensim stepping\n";
		std::cerr << e.what();

		throw;
	}
}

void NRPOpensimCommunicationController::initialize(const json &data, EngineGrpcServer::lock_t &lock)
{
	std::cout <<"From nrp_opensim_communication_controller.cpp!!!\n\n";
	ConfigStorage confDat(data);
	OpensimGrpcConfig conf(confDat);

	double waitTime = conf.maxOpensimWorldLoadTime();
	if(conf.maxOpensimWorldLoadTime() <= 0)
		waitTime = std::numeric_limits<double>::max();

	// Allow devices to register
	lock.unlock();

	// Wait until world plugin loads and forces a load of all other plugins
	while(this->_stepController == nullptr ? 1 : !this->_stepController->finishWorldLoading())
	{
		// Wait for 100ms before retrying
		waitTime -= 0.1;
		usleep(100*1000);

		if(waitTime <= 0)
		{
			lock.lock();

			const auto errMsg = "Couldn't load world";
            throw std::runtime_error(errMsg);
		}
	}

	lock.lock();
}

void NRPOpensimCommunicationController::shutdown(const json&)
{
	// Do nothing
}

NRPOpensimCommunicationController::NRPOpensimCommunicationController(const std::string &address)
    : EngineGrpcServer(address)
{}

NRPOpensimCommunicationController::NRPOpensimCommunicationController(const std::string &serverURL, const std::string &engineName, const std::string &registrationURL)
    : EngineGrpcServer(serverURL, engineName, registrationURL)
{}

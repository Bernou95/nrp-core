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

#include "nrp_simulation/simulation/simulation_manager.h"

#include "nrp_general_library/utils/file_finder.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_simulation/config/cmake_conf.h"

#include <iostream>
#include <fstream>
#include <future>
#include <spdlog/spdlog.h>


cxxopts::Options SimulationParams::createStartParamParser()
{
	cxxopts::Options opts(SimulationParams::NRPProgramName.data(), SimulationParams::ProgramDescription.data());
	opts.add_options()
	        (SimulationParams::ParamHelpLong.data(), SimulationParams::ParamHelpDesc.data(),
	         cxxopts::value<SimulationParams::ParamHelpT>()->default_value("0"))
	        (SimulationParams::ParamSimCfgFileLong.data(), SimulationParams::ParamSimCfgFileDesc.data(),
	         cxxopts::value<SimulationParams::ParamSimCfgFileT>())
	        (SimulationParams::ParamPluginsLong.data(), SimulationParams::ParamPluginsDesc.data(),
	         cxxopts::value<SimulationParams::ParamPluginsT>()->default_value({}));

	return opts;
}

nlohmann::json SimulationParams::parseJSONFile(const std::string &fileName)
{
	// Try to open file
	std::ifstream cfgFile(fileName);
	if(!cfgFile.is_open())
	{
		throw std::invalid_argument("Error: Could not open config file " + fileName);
	}

	// Parse JSON from file
	nlohmann::json cfgJSON;
	try
	{
		cfgFile >> cfgJSON;
	}
	catch(std::exception &e)
	{
		throw std::invalid_argument("Error: Could not parse config file " + fileName + "\n" + e.what());
	}

	return cfgJSON;
}


SimulationManager::SimulationManager(const jsonSharedPtr &simulationConfig)
    : _simConfig(simulationConfig)
{}

SimulationManager::~SimulationManager()
{
	// Stop running threads
	this->shutdownLoop(this->acquireSimLock()); //TODO Refactor: why twice shutdownLoop?

	// Prevent future sim initialization or loop execution
	this->_internalLock.lock();
	auto simLock = this->acquireSimLock(); //TODO Refactor: review locks usage

	// Ensure that any potentially created loops are stopped
	this->shutdownLoop(simLock);

	// Keep locked until everything is destructed
	simLock.release();
}


SimulationManager SimulationManager::createFromParams(const cxxopts::ParseResult &args)
{
    jsonSharedPtr simConfig = nullptr;

	// Get file names from start params
	std::string simCfgFileName;

	try
	{
		simCfgFileName = args[SimulationParams::ParamSimCfgFile.data()].as<SimulationParams::ParamSimCfgFileT>();
	}
	catch(std::domain_error&)
	{
		// If no simulation file name is present, return empty config
		return SimulationManager(simConfig);
	}


	simConfig.reset(new nlohmann::json(SimulationParams::parseJSONFile(simCfgFileName)));

	json_utils::validate_json(*simConfig, "https://neurorobotics.net/simulation.json#Simulation");

	// Set default values

	json_utils::set_default<std::vector<std::string>>(*simConfig, "TransceiverFunctionConfigs", std::vector<std::string>());
    json_utils::set_default<std::vector<std::string>>(*simConfig, "PreprocessingFunctionConfigs", std::vector<std::string>());

	return SimulationManager(simConfig);
}

SimulationLoopConstSharedPtr SimulationManager::simulationLoop() const
{
	return this->_loop;
}

SimulationManager::sim_lock_t SimulationManager::acquireSimLock()
{
	auto retval = sim_lock_t(this->_simulationLock);
	return retval;
}

jsonSharedPtr SimulationManager::simulationConfig(const sim_lock_t&)
{
	return this->_simConfig;
}

jsonConstSharedPtr SimulationManager::simulationConfig() const
{
	return this->_simConfig;
}

void SimulationManager::initSimulationLoop(const EngineLauncherManagerConstSharedPtr &engineLauncherManager,
                                                                    const MainProcessLauncherManager::const_shared_ptr &processLauncherManager,
                                                                    sim_lock_t &simLock)
{
	// Make sure initLock is retrieved before simLock
	if(simLock.owns_lock())
		simLock.unlock();

	sim_lock_t initLock(this->_internalLock);

	simLock.lock();

	// Create and initialize loop
	spdlog::info("Initializing simulation loop");
	this->_loop.reset(new SimulationLoop(this->createSimLoop(engineLauncherManager, processLauncherManager)));

	//sleep(10);

	this->_loop->initLoop();
}

bool SimulationManager::isRunning() const
{
	return this->_loop != nullptr && this->_runningSimulation;
}

void SimulationManager::stopSimulation(const sim_lock_t&)
{
	this->_runningSimulation = false;
}

bool SimulationManager::runSimulationUntilTimeout(sim_lock_t &simLock)
{
	if(this->_loop == nullptr)
		return false;

	// Lock and unlock mutex before/after every timestep. This allows server threads to execute
	this->_runningSimulation = true;
	simLock.unlock();

	sim_lock_t internalLock(this->_internalLock);

	bool hasTimedOut = false;

	while(1)
	{
		// Check whether the simLoop was stopped by any server threads
		simLock.lock();

<<<<<<< HEAD
<<<<<<< HEAD
		//hasTimedOut = hasSimTimedOut(this->_loop->getSimTime(), toSimulationTime<unsigned, std::ratio<1>>(this->_simConfig->at("SimulationTimeout")));
		hasTimedOut = hasSimTimedOut(this->_loop->getSimTime(), toSimulationTime<float, std::ratio<1>>(this->_simConfig->at("SimulationTimeout")));

=======
<<<<<<< HEAD
=======
>>>>>>> 6d4c0601f0eecb37b4ff9a94480f3c2e251e244b
<<<<<<< HEAD
		//hasTimedOut = hasSimTimedOut(this->_loop->getSimTime(), toSimulationTime<unsigned, std::ratio<1>>(this->_simConfig->at("SimulationTimeout")));
		hasTimedOut = hasSimTimedOut(this->_loop->getSimTime(), toSimulationTime<float, std::ratio<1>>(this->_simConfig->at("SimulationTimeout")));

=======
		hasTimedOut = hasSimTimedOut(this->_loop->getSimTime(), toSimulationTime<unsigned, std::ratio<1>>(this->_simConfig->at("SimulationTimeout")));
>>>>>>> dc7adbacd5ccbb81923578be0a04c242f0c3a763
=======
		hasTimedOut = hasSimTimedOut(this->_loop->getSimTime(), toSimulationTime<unsigned, std::ratio<1>>(this->_simConfig->at("SimulationTimeout")));
>>>>>>> dc7adbacd5ccbb81923578be0a04c242f0c3a763
<<<<<<< HEAD
>>>>>>> 6d4c0601f0eecb37b4ff9a94480f3c2e251e244b
=======
>>>>>>> 6d4c0601f0eecb37b4ff9a94480f3c2e251e244b
		if(!this->isRunning() || hasTimedOut)
			break;

		SimulationTime timeStep = toSimulationTime<float, std::ratio<1>>(this->_simConfig->at("SimulationTimestep"));

		this->_loop->runLoop(timeStep);

		simLock.unlock();
		std::this_thread::yield();			// Give any server threads that may wish to lock the simulation a chance to execute TODO: Use better command than sched_yield, which slows down execution significantly
	}

	this->_runningSimulation = false;

	return hasTimedOut;
}

bool SimulationManager::runSimulation(const SimulationTime secs, sim_lock_t &simLock)
{
	if(this->_loop == nullptr)
		return false;

	this->_runningSimulation = true;
	simLock.unlock();

	sim_lock_t internalLock(this->_internalLock);

	this->_runningSimulation = true;
	const auto endTime = this->_loop->getSimTime() + secs;
	while(1)
	{
		simLock.lock();

		if(!this->isRunning() || endTime < this->_loop->getSimTime())
			break;

		SimulationTime timeStep = toSimulationTime<float, std::ratio<1>>(this->_simConfig->at("SimulationTimestep"));

		this->_loop->runLoop(timeStep);

		simLock.unlock();
		std::this_thread::yield();			// Give any server threads that may wish to lock the simulation a chance to execute TODO: Use better command than sched_yield, which slows down execution significantly
	}

	this->_runningSimulation = false;

	return endTime <= this->_loop->getSimTime();
}

void SimulationManager::shutdownLoop(const SimulationManager::sim_lock_t&)
{
	try {
		if(this->_loop != nullptr) {
			this->_loop->shutdownLoop();

			this->_loop = nullptr;
			this->_runningSimulation = false;
		}
	}
	catch(NRPException &e) {
		throw NRPException::logCreate(e, "SimulationManager: Loop shutdown has FAILED");
	}

}

bool SimulationManager::isSimInitializing()
{
	if(this->_loop != nullptr)
		return false;

	sim_lock_t lock(this->_internalLock, std::defer_lock);
	return lock.try_lock();
}

SimulationLoop SimulationManager::createSimLoop(const EngineLauncherManagerConstSharedPtr &engineManager, const MainProcessLauncherManager::const_shared_ptr &processLauncherManager)
{
	SimulationLoop::engine_interfaces_t engines;
	auto &engineConfigs = this->_simConfig->at("EngineConfigs");

	// Create all engines required by simConfig
	engines.reserve(engineConfigs.size());
	for(auto &engineConfig : engineConfigs)
	{
		// Get engine launcher associated with type
		const std::string engineType = engineConfig.at("EngineType");
		auto engineLauncher = engineManager->findLauncher(engineType);
		if(engineLauncher == nullptr)
		{
			const auto errMsg = "Failed to find engine interface \"" + engineType + "\"";
			spdlog::error(errMsg);

			throw std::invalid_argument(errMsg);
		}

		// Create and launch engine
		try
		{
			engines.push_back(engineLauncher->launchEngine(engineConfig, processLauncherManager->createProcessLauncher(this->_simConfig->at("ProcessLauncherType"))));
		}
		catch(std::exception &e)
		{
			throw NRPException::logCreate(e, "Failed to launch engine interface \"" + engineLauncher->engineType() + "\"");
		}
	}

	return SimulationLoop(this->_simConfig, engines);
}

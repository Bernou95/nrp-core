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
	        (SimulationParams::ParamServCfgFileLong.data(), SimulationParams::ParamServCfgFileDesc.data(),
	         cxxopts::value<SimulationParams::ParamServCfgFileT>())
	        (SimulationParams::ParamSimCfgFileLong.data(), SimulationParams::ParamSimCfgFileDesc.data(),
	         cxxopts::value<SimulationParams::ParamSimCfgFileT>())
	        (SimulationParams::ParamPluginsLong.data(), SimulationParams::ParamPluginsDesc.data(),
	         cxxopts::value<SimulationParams::ParamPluginsT>()->default_value({}))
	        (SimulationParams::ParamExpManPipeLong.data(), SimulationParams::ParamExpManPipeDesc.data(),
	         cxxopts::value<SimulationParams::ParamExpManPipeT>()->default_value({}));

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


SimulationManager::SimulationManager(const ServerConfigConstSharedPtr &serverConfig, const SimulationConfigSharedPtr &simulationConfig)
    : _simConfig(simulationConfig),
      _serverConfig(serverConfig)
{}

SimulationManager::~SimulationManager()
{
	// Stop running threads
	this->shutdownLoop(this->acquireSimLock());

	// Prevent future sim initialization or loop execution
	this->_internalLock.lock();
	auto simLock = this->acquireSimLock();

	// Ensure that any potentially created loops are stopped
	this->shutdownLoop(simLock);

	// Keep locked until everything is destructed
	simLock.release();
}


SimulationManager SimulationManager::createFromParams(const cxxopts::ParseResult &args)
{
	ServerConfigConstSharedPtr serverConfig = nullptr;
	SimulationConfigSharedPtr simConfig = nullptr;

	// Get file names from start params
	std::string servCfgFileName;
	std::string simCfgFileName;

	try
	{	servCfgFileName = args[SimulationParams::ParamServCfgFile.data()].as<SimulationParams::ParamServCfgFileT>();	}
	catch(std::domain_error&)
	{
		servCfgFileName = FileFinder::findFile(NRP_SERVER_CONFIG_FILE_NAME, NRP_SERVER_CONFIG_DIRS);
		if(servCfgFileName.empty())
			throw std::runtime_error("Could not find server config file");
	}

	// Parse JSON config from file
	nlohmann::json servCfgJSON = SimulationParams::parseJSONFile(servCfgFileName);
	serverConfig.reset(new ServerConfig(servCfgJSON));


	try
	{
		simCfgFileName = args[SimulationParams::ParamSimCfgFile.data()].as<SimulationParams::ParamSimCfgFileT>();
	}
	catch(std::domain_error&)
	{
		// If no simulation file name is present, return empty config
		return SimulationManager(serverConfig, simConfig);
	}

	nlohmann::json simCfgJSON = SimulationParams::parseJSONFile(simCfgFileName);
	simConfig.reset(new SimulationConfig(simCfgJSON));


	return SimulationManager(serverConfig, simConfig);
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

SimulationConfigSharedPtr SimulationManager::simulationConfig(const sim_lock_t&)
{
	return this->_simConfig;
}

SimulationConfigConstSharedPtr SimulationManager::simulationConfig() const
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

	while(1)
	{
		// Check whether the simLoop was stopped by any server threads
		simLock.lock();

		if(!this->isRunning() || hasSimTimedOut(this->_loop->getSimTime(), this->_simConfig->simulationTimeOut()))
			break;

		this->_loop->runLoop(this->_serverConfig->serverTimestep());

		simLock.unlock();
		std::this_thread::yield();			// Give any server threads that may wish to lock the simulation a chance to execute TODO: Use better command than sched_yield, which slows down execution significantly
	}

	this->_runningSimulation = false;

	return hasSimTimedOut(this->_loop->getSimTime(), this->_simConfig->simulationTimeOut());
}

bool SimulationManager::runSimulation(const float secs, sim_lock_t &simLock)
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

		this->_loop->runLoop(this->_serverConfig->serverTimestep());

		simLock.unlock();
		std::this_thread::yield();			// Give any server threads that may wish to lock the simulation a chance to execute TODO: Use better command than sched_yield, which slows down execution significantly
	}

	this->_runningSimulation = false;

	return endTime <= this->_loop->getSimTime();
}

void SimulationManager::shutdownLoop(const SimulationManager::sim_lock_t&)
{
	this->_loop = nullptr;
	this->_runningSimulation = false;
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
	auto &engineConfigs = this->_simConfig->engineConfigs();

	// Create all engines required by simConfig
	engines.reserve(engineConfigs.size());
	for(auto &engineConfig : engineConfigs)
	{
		// Get engine type
		nlohmann::json engineData = static_cast<const nlohmann::json &>(engineConfig);
		auto engineTypeIterator = engineData.find(EngineConfigConst::EngineType.m_data);
		if(engineTypeIterator == engineData.end())
			throw NRPException::logCreate("Improperly formatted engine config. Couldn't find EngineType specification");

		// Get engine launcher associated with type
		const std::string engineType = *engineTypeIterator;
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
			engines.push_back(engineLauncher->launchEngine(engineConfig, processLauncherManager->createProcessLauncher(this->_serverConfig->processLauncherType())));
		}
		catch(std::exception &e)
		{
			throw NRPException::logCreate(e, "Failed to launch engine interface \"" + engineLauncher->engineType() + "\"");
		}
	}

	return SimulationLoop(this->_simConfig, engines);
}

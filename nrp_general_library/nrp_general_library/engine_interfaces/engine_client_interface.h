/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
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

#ifndef ENGINE_CLIENT_INTERFACE_H
#define ENGINE_CLIENT_INTERFACE_H

#include "nrp_general_library/device_interface/device.h"
#include "nrp_general_library/process_launchers/process_launcher.h"
#include "nrp_general_library/utils/fixed_string.h"
#include "nrp_general_library/utils/ptr_templates.h"
#include "nrp_general_library/utils/time_utils.h"
#include "nrp_general_library/utils/json_schema_utils.h"

#include <concepts>
#include <set>
#include <vector>

class EngineClientInterface;
class EngineLauncherInterface;

template<class T>
concept ENGINE_C = requires (nlohmann::json  &engineConfig, ProcessLauncherInterface::unique_ptr &&launcher) {
    std::derived_from<T, EngineClientInterface>;
    //std::constructible_from<T, nlohmann::json&, ProcessLauncherInterface::unique_ptr&&>;
    //{ T(engineConfig, std::move(launcher)) };
};

/*!
 * \brief Interface to engines
 */
class EngineClientInterface
        : public PtrTemplates<EngineClientInterface>
{
		/*! \brief DeviceInterfaceConstSharedPtr comparison. Used for set sorting */
		struct CompareDevInt : public std::less<>
		{
			public: bool operator()(const DeviceInterfaceConstSharedPtr &lhs, const DeviceInterfaceConstSharedPtr &rhs) const
			{	return lhs->name() < rhs->name();	}

			public: bool operator()(const DeviceInterfaceConstSharedPtr &lhs, const std::string &name) const
			{	return lhs->name() < name;	}

			public: bool operator()(const std::string &name, const DeviceInterfaceConstSharedPtr &rhs) const
			{	return name < rhs->name();	}
		};

	public:
		using device_identifiers_set_t = std::set<DeviceIdentifier>;
		using devices_t = std::vector<DeviceInterfaceConstSharedPtr>;
		using devices_set_t = std::set<DeviceInterfaceConstSharedPtr, CompareDevInt>;
		using devices_ptr_t = std::vector<DeviceInterface*>;

		explicit EngineClientInterface(ProcessLauncherInterface::unique_ptr &&launcher);
		virtual ~EngineClientInterface();

		/*!
		 * \brief Get Engine Name
		 * \return Returns engine name
		 */
		virtual const std::string engineName() const = 0;

		/*!
		 * \brief Get engine config data
		 */
		virtual const nlohmann::json &engineConfig() const = 0;

		/*!
        * \brief Get engine config data
        */
        virtual nlohmann::json &engineConfig() = 0;

        /*!
         * \brief Get all Engine Process Startup parameters.
         */
        virtual const std::vector<std::string> engineProcStartParams() const = 0;

        /*!
         * \brief Get all Engine Process Environment variables.
         */
        virtual const std::vector<std::string> engineProcEnvParams() const = 0;

		/*!
		 * \brief Launch the engine
		 * \return Returns engine process ID on success, throws on failure
		 */
		virtual pid_t launchEngine();

		/*!
		 * \brief Initialize engine
		 * \return Returns SUCCESS if no error was encountered
		 * \throw Throws on error
		 */
		virtual void initialize() = 0;

		/*!
		 * \brief Shutdown engine
		 * \return Return SUCCESS if no error was encountered
		 * \throw Throws on error
		 */
		virtual void shutdown() = 0;

		/*!
		 * \brief Get engine timestep (in seconds)
		 * \throw Throws on error
		 */
		virtual SimulationTime getEngineTimestep() const = 0;

		/*!
		 * \brief Get current engine time (in seconds)
		 * \return Returns engine time
		 * \throw Throws on error
		 */
		virtual SimulationTime getEngineTime() const = 0;

        /*!
         * \brief Get json schema for this specific engine type.
         * \return Returns URI of engine schema
         * \throw Throws on error
         */
        virtual const std::string engineSchema() const = 0;

		/*!
		 * \brief Starts a single loop step in a separate thread.
		 * EngineClientInterface::waitForStepCompletion() can be used to join threads again
		 * \param timeStep Time (in seconds) of a single step
		 * \throw Throws on error
		 */
		virtual void runLoopStep(SimulationTime timeStep) = 0;

		/*!
		 * \brief Wait until step has been completed, at most timeOut seconds
		 * \param timeOut Wait for at most timeOut seconds
		 * \return Returns SUCCESS if step has completed before timeOut, ERROR otherwise
		 * \throw Throws on error
		 */
		virtual void waitForStepCompletion(float timeOut) = 0;

		/*!
		 * \brief Gets requested devices from engine and updates _deviceCache with the results
		 * Uses getDevicesFromEngine override for the actual communication
		 * \param deviceNames All requested names. NOTE: can also include IDs of other engines. A check must be added that only the corresponding IDs are retrieved
		 * \return Returns all devices returned by the engine
		 */
		const devices_t &updateDevicesFromEngine(const device_identifiers_set_t &deviceIdentifiers);

		/*!
		 * \brief get cached engine devices
		 */
		constexpr const devices_t &getCachedDevices() const
		{	return this->_deviceCache;	}

		/*!
		 * \brief Sends devices to engine
		 * \param devicesArray Array of devices that will be send to the engine
		 * \return Returns SUCCESS if all devices could be handles, ERROR otherwise
		 * \throw Throws on error
		 */
		virtual void sendDevicesToEngine(const devices_ptr_t &devicesArray) = 0;

		/*!
		 * \brief Update _deviceCache from devices
		 *
		 * If the device with a particular name is already in the cache, the function will
		 * replace it. If the device isn't in the cache, the function will insert it.
		 *
		 * \param devs Devices to insert
		 */
		void updateCachedDevices(devices_set_t &&devs);

	protected:

		/*!
		 * \brief Gets requested devices from engine
		 * \param deviceNames All requested devince ids
		 * \return Returns all requested devices
		 * \throw Throws on error
		 */
		virtual devices_set_t getDevicesFromEngine(const device_identifiers_set_t &deviceIdentifiers) = 0;

		/*!
		 * \brief Process Launcher. Will be used to stop process at end
		 */
		ProcessLauncherInterface::unique_ptr _process;

		/*!
		 * \brief Engine device cache. Stores retrieved devices
		 */
		devices_t _deviceCache;
};

using EngineClientInterfaceSharedPtr = EngineClientInterface::shared_ptr;
using EngineClientInterfaceConstSharedPtr = EngineClientInterface::const_shared_ptr;

class EngineLauncherInterface
        : public PtrTemplates<EngineLauncherInterface>
{
	public:
		using engine_type_t = decltype(DeviceIdentifier::Type);

		EngineLauncherInterface(const engine_type_t &engineType);
		virtual ~EngineLauncherInterface() = default;

		const engine_type_t &engineType() const;
		virtual EngineClientInterfaceSharedPtr launchEngine(nlohmann::json  &engineConfig, ProcessLauncherInterface::unique_ptr &&launcher) = 0;

		/*!
		 *	\brief Compare EngineLaunchers according to _engineType
		 */
		auto operator<=>(const EngineLauncherInterface&) const = default;
		bool operator==(const EngineLauncherInterface&) const = default;
	private:
		/*!
		 * \brief Engine Type
		 */
		const engine_type_t _engineType;
};

using EngineLauncherInterfaceSharedPtr = EngineLauncherInterface::shared_ptr;
using EngineLauncherInterfaceConstSharedPtr = EngineLauncherInterface::const_shared_ptr;

/*!
 *	\brief Base class for all Engines
 *	\tparam ENGINE Final derived engine class
 */
template<class ENGINE, FixedString SCHEMA>
class EngineClient
        : public EngineClientInterface
{
	public:
		using engine_t = ENGINE;

		/*!
		 * \brief Class for launching engine
		 * \tparam ENGINE_TYPE Default engine type
		 */
		template<FixedString ENGINE_TYPE>
		class EngineLauncher
		        : public EngineLauncherInterface
		{
			public:
				EngineLauncher()
				    : EngineLauncherInterface(ENGINE_TYPE.m_data)
				{}

				EngineLauncher(const engine_type_t &engineType)
				    : EngineLauncherInterface(engineType)
				{}

				~EngineLauncher() override = default;

				/*!
				 * \brief Launches an engine. Configures config and forks a new child process for the engine
				 * \param engineConfig Engine Configuration
				 * \param launcher Process Forker
				 * \return Returns pointer to EngineClientInterface
				 */
				EngineClientInterfaceSharedPtr launchEngine(nlohmann::json &engineConfig, ProcessLauncherInterface::unique_ptr &&launcher) override
				{
					EngineClientInterfaceSharedPtr engine(new ENGINE(engineConfig, std::move(launcher)));
					if(engine->launchEngine() == 0)
					{	/*TODO: Handle process forking error*/	}

					return engine;
				}
		};

		/*!
		 * \brief Constructor
		 * \param engineConfig Engine Configuration
		 * \param launcher Process Forker
		 */
		EngineClient(nlohmann::json  &engineConfig, ProcessLauncherInterface::unique_ptr &&launcher)
		    : EngineClientInterface(std::move(launcher)),
		      engineConfig_(engineConfig)
		{
		    // validate engine config
            json_utils::validate_json(this->engineConfig(), this->engineSchema());

		    // setting process start and env params to an empty vector since this can't be done from json schema
		    setDefaultProperty<std::vector<std::string>>("EngineProcStartParams", std::vector<std::string>());
            setDefaultProperty<std::vector<std::string>>("EngineEnvParams", std::vector<std::string>());
		}

		~EngineClient() override = default;

        const std::string engineName() const override final
        { return this->engineConfig().at("EngineName"); }

		SimulationTime getEngineTimestep() const override final
		{
			// We need to cast floating-point seconds to integers with units of SimulationTime type
			return toSimulationTime<float, std::ratio<1>>(this->engineConfig().at("EngineTimestep"));
		}

		/*!
		 * \brief Get Engine Configuration
		 */
		const nlohmann::json &engineConfig() const override final
		{	return engineConfig_;	}

        /*!
         * \brief Get Engine Configuration
         */
        nlohmann::json &engineConfig() override final
        {	return engineConfig_;	}

        /*!
         * \brief Get json schema for this engine type
         */
        const std::string engineSchema() const override final
        {   return SCHEMA.m_data;   }

    protected:

        /*!
        * \brief Attempts to set a default value for a property in the engine configuration. If the property has been already
         * set either in the engine configuration file or from the engine schema, its value is not overwritten.
        * \param key Name of the property to be set
        * \param value Default value to set
        */
        template<class T>
        void setDefaultProperty(std::string key, T value)
        {
            json_utils::set_default<T>(this->engineConfig(), key, value);
        }

    private:

        nlohmann::json engineConfig_;
};


/*! \page engines "Engines"
Engines are the core aspect of NRP Simulation. They run the actual simulation software, with the SimulationLoop and TransceiverFunctions merely being a way to synchronize and
exchange data between them. An Engine can run any type of software, from physics engines to brain simulators.

From the NRP's perspective, the core component of the engine is its communication interface, which allows it to communicate with the SimulationLoop. Different engine types can
have different communication protocols. Nevertheless, all protocols are envisioned as a server-client architecture, with the Engine server running as a separate process, and a
client running inside the NRPSimulation. As such, all Engines must at least support the following functionality:

- LaunchEngine: A function to launch the engine process. This will usually in some way use the ProcessLauncherInterface
- Initialize: A function that initializes the engine after launch
- RunLoopStep: A function that will advance the engine for a given timestep
- RequestOutputDevices: A function to retrieve Device data from the Engine
- HandleInputDevices: A function to handle incoming Device data
- Shutdown: A function that gracefully stops the Engine

The \ref index "Main Page" has a list of currently supported Engines.

Creating new engines is a process that requires multiple components to work together. Should you be interested in implementing your own engine, a good starting point is
\ref tutorial_engine_creation "this tutorial". An easiser approach is to use one of our already implemented Grpc communication protocol and adapt it to your simulator.
The tutorial can be found \ref grpc_engine_creation "here".

\subsection Engine Launchers

An EngineLauncher is in charge of properly launching an engine using a given ProcessLauncher and Engine Configuration. we have provided an already prepared class that
can be used out-of-the-box, with any new Engine.
\code{.cpp}
// Define the EngineLauncher.
using NewEngineLauncher = NewEngine::EngineLauncher<NewEngineConfig::DefEngineType>;
\endcode

A new Engine library can use such a `NewEngineLauncher` to make it plugin compatible. Look under \ref plugin_system for additional details.

Note that we assign this EngineLauncher the name specified in NewEngineConfig::DefEngineType. Thus, a user can select this engine in the main simulation configuration file by
setting an Engine's EngineType parameter to the string specified in `NewEngineConfig::DefEngineType`. For details about setting up a simulation configuration file, look
\ref simulation_config "here".

For details about how to create a `NewEngineConfig` class, see section \ref config.

\bold Should an engine require more complex startup routines, consider overriding the EngineClient's EngineClientInterface::launchEngine() function. Do not modify the default
\bold EngineLauncher, as its only purpose is to construct the Engine class and then call the above function.
 */

#endif // ENGINE_CLIENT_INTERFACE_H

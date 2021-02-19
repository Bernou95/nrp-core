/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020 Michael Zechmair
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

#ifndef OPENSIM_GRPC_CONFIG_H
#define OPENSIM_GRPC_CONFIG_H

#include "nrp_grpc_engine_protocol/config/engine_grpc_config.h"
#include "nrp_general_library/utils/ptr_templates.h"

struct OpensimGrpcConfigConst
{
	/*!
	 * \brief OpensimGrpcConfig Type
	*/
	static constexpr FixedString ConfigType = "OpensimGrpcConfig";

	/*!
	 * \brief Plugins to load into opensim
	*/
	static constexpr FixedString OpensimPlugins = "OpensimPlugins";
	static const std::vector<std::string> DefOpensimPlugins;

	static constexpr FixedString OpensimRNGSeed = "OpensimRNGSeed";
	static const size_t DefOpensimRNGSeed;

	/*!
	 * \brief Opensim Start Parameters Argument for plugins
	*/
	static constexpr std::string_view OpensimPluginArg = "-s";

	/*!
	 * \brief Opensim Start Parameters Argument for random seed value
	*/
	static constexpr std::string_view OpensimRNGSeedArg = "--seed";

	/*!
	 * \brief Maximum time (in seconds) to wait for the NRPCommunicatioPlugin to load the world file, and for the world sdf to be loaded. 0 means it will wait forever
	*/
	static constexpr FixedString MaxOpensimWorldLoadTime = "WorldLoadTime";
	static constexpr unsigned int DefMaxOpensimWorldLoadTime = 20;

	/*!
	 * \brief Opensim SDF World file
	*/
	static constexpr FixedString OpensimWorldFileName = "OpensimWorldFileName";
	static constexpr std::string_view DefOpensimWorldFileName = "";

	using GPropNames = PropNames<OpensimPlugins, OpensimRNGSeed, MaxOpensimWorldLoadTime, OpensimWorldFileName>;
};

/*!
 *  \brief Configuration for Opensim physics engine
*/
class OpensimGrpcConfig
		: public EngineGRPCConfig<OpensimGrpcConfig, OpensimGrpcConfigConst::GPropNames,
			std::vector<std::string>, std::size_t, unsigned int, std::string>,
		public OpensimGrpcConfigConst,
		public PtrTemplates<OpensimGrpcConfig>
{
	public:
		// Default engine values. Copied from EngineConfigConst
		static constexpr FixedString DefEngineType = "opensim_grpc";
		static constexpr std::string_view DefEngineName = "opensim_engine";
		//static const string_vector_t DefEngineProcEnvParams;
		static constexpr std::string_view DefEngineProcCmd = "/usr/bin/gzserver";
		//static const string_vector_t DefEngineProcStartParams;

		/*!
		 * \brief Constructor. Takes configuration data from the main SimulationConfig class. 
			Will register itself with said class, so that anytime the configuration is saved,
			any changes made by OpensimGrpcConfig are passed along
		 * \param config Opensim Configuration, taken from the SimulationConfig class
		*/
		OpensimGrpcConfig(EngineConfigConst::config_storage_t &config);

		std::string &opensimWorldFile();
		const std::string &opensimWorldFile() const;

		std::vector<std::string> &opensimPlugins();
		const std::vector<std::string> &opensimPlugins() const;

		size_t &opensimRNGSeed();
		const size_t &opensimRNGSeed() const;

		unsigned int &maxOpensimWorldLoadTime();
		const unsigned int &maxOpensimWorldLoadTime() const;

		string_vector_t allEngineProcEnvParams() const override;
		string_vector_t allEngineProcStartParams() const override;

	private:
		/*!
		 * \brief Checks that a given environment variable is properly formated. Should be something along the lines of "ENV_VARIABLE=VALUE", meaning there should be an '=' present in the string, and some character(s) before and after
		 * \param envVariable Environment variable to check
		 * \return Returns true if format is correct
		*/
		static bool checkEnvVariableFormat(const std::string &envVariable);

};

using OpensimGrpcConfigSharedPtr = OpensimGrpcConfig::shared_ptr;
using OpensimGrpcConfigConstSharedPtr = OpensimGrpcConfig::const_shared_ptr;

#endif // OPENSIM_GRPC_CONFIG_H

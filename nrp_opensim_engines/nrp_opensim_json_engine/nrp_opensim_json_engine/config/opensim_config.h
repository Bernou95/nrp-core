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

#ifndef OPENSIM_CONFIG_H
#define OPENSIM_CONFIG_H

#include "nrp_json_engine_protocol/config/engine_json_config.h"
#include "nrp_general_library/utils/ptr_templates.h"

#include "nrp_opensim_json_engine/config/cmake_constants.h"

struct OpensimConfigConst
{
	/*!
<<<<<<< HEAD
	 * \brief OpensimConfig Type
=======
	 * \brief PythonConfig Type
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37
	 */
	static constexpr FixedString ConfigType = "OpensimConfig";

	/*!
	 * \brief Python File (contains the python engine script)
	 */
<<<<<<< HEAD
	static constexpr FixedString OpensimRunPy = "OpensimRunPy";
	static constexpr std::string_view DefOpensimRunPy = "";

	static constexpr FixedString OpensimFileName = "OpensimFileName";
	static constexpr std::string_view DefOpensimFileName = "";

=======
	static constexpr FixedString PythonFileName = "PythonFileName";
	static constexpr std::string_view DefPythonFileName = "";
	/*!
	 * \brief Opensim File (contains the opensim engine script)
	 */
	static constexpr FixedString OpensimFileName = "OpensimFileName";
	static constexpr std::string_view DefOpensimFileName = "";
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37
	/*!
	 * \brief After the server executes the init file, this status flag will either be 1 for success or 0 for fail. If the execution fails, a JSON message with more details will be passed as well (under InitFileErrorMsg).
	 */
	static constexpr std::string_view InitFileExecStatus = "InitExecStatus";

	/*!
	 * \brief If the init file could not be parsed, the python error message will be stored under this JSON property name
	 */
	static constexpr std::string_view InitFileErrorMsg = "Message";

<<<<<<< HEAD
	using OpPropNames = PropNames<OpensimRunPy, OpensimFileName>;
};

class OpensimConfig
        : public EngineJSONConfig<OpensimConfig, OpensimConfigConst::OpPropNames, std::string, std::string>,
=======
	using PPropNames = PropNames<PythonFileName, OpensimFileName>;
};

class OpensimConfig
        : public EngineJSONConfig<OpensimConfig, OpensimConfigConst::PPropNames, std::string, std::string>,
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37
          public PtrTemplates<OpensimConfig>,
          public OpensimConfigConst
{
	public:
		// Default engine values. Copies from EngineConfigConst
		static constexpr FixedString DefEngineType = "opensim_json";
		static constexpr std::string_view DefEngineName = "opensim_engine";
		//static const string_vector_t DefEngineProcEnvParams;
<<<<<<< HEAD
		static constexpr std::string_view DefEngineProcCmd = NRP_OPENSIM_EXECUTABLE_PATH;
=======
		static constexpr std::string_view DefEngineProcCmd = NRP_PYTHON_EXECUTABLE_PATH;
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37
		//static const string_vector_t DefEngineProcStartParams;

		OpensimConfig(EngineConfigConst::config_storage_t &config);
		OpensimConfig(const nlohmann::json &data);

<<<<<<< HEAD
		const std::string &opensimRunPy() const;
		std::string &opensimRunPy();
=======
		const std::string &pythonFileName() const;
		std::string &pythonFileName();
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37

		const std::string &opensimFileName() const;
		std::string &opensimFileName();

		//string_vector_t allEngineProcEnvParams() const override;
		string_vector_t allEngineProcStartParams() const override;
};

using OpensimConfigSharedPtr = OpensimConfig::shared_ptr;
using OpensimConfigConstSharedPtr = OpensimConfig::const_shared_ptr;

<<<<<<< HEAD
#endif // OPENSIM_CONFIG_H
=======
#endif // PYTHON_CONFIG_H
>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37

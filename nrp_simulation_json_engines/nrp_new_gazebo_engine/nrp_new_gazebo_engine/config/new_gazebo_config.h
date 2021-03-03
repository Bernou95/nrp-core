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

#ifndef NEW_GAZEBO_CONFIG_H
#define NEW_GAZEBO_CONFIG_H

#include "nrp_json_engine_protocol/config/engine_json_config.h"
#include "nrp_general_library/utils/ptr_templates.h"

#include "nrp_new_gazebo_engine/config/cmake_constants.h"

struct NewGazeboConfigConst
{
	/*!
	 * \brief NewGazeboConfig Type
	 */
	static constexpr FixedString ConfigType = "NewGazeboConfig";

	/*!
	 * \brief Python File (contains the python engine script)
	 */
	static constexpr FixedString NewGazeboRunPy = "NewGazeboRunPy";
	static constexpr std::string_view DefNewGazeboRunPy = "";

	static constexpr FixedString NewGazeboFileName = "NewGazeboFileName";
	static constexpr std::string_view DefNewGazeboFileName = "";

	/*!
	 * \brief After the server executes the init file, this status flag will either be 1 for success or 0 for fail. If the execution fails, a JSON message with more details will be passed as well (under InitFileErrorMsg).
	 */
	static constexpr std::string_view InitFileExecStatus = "InitExecStatus";

	/*!
	 * \brief If the init file could not be parsed, the python error message will be stored under this JSON property name
	 */
	static constexpr std::string_view InitFileErrorMsg = "Message";

	using OpPropNames = PropNames<NewGazeboRunPy, NewGazeboFileName>;
};

class NewGazeboConfig
        : public EngineJSONConfig<NewGazeboConfig, NewGazeboConfigConst::OpPropNames, std::string, std::string>,
          public PtrTemplates<NewGazeboConfig>,
          public NewGazeboConfigConst
{
	public:
		// Default engine values. Copies from EngineConfigConst
		static constexpr FixedString DefEngineType = "new_gazebo";
		static constexpr std::string_view DefEngineName = "new_gazebo_engine";
		//static const string_vector_t DefEngineProcEnvParams;
		static constexpr std::string_view DefEngineProcCmd = NEW_GAZEBO_EXECUTABLE_PATH;
		//static const string_vector_t DefEngineProcStartParams;

		NewGazeboConfig(EngineConfigConst::config_storage_t &config);
		NewGazeboConfig(const nlohmann::json &data);

		const std::string &newGazeboRunPy() const;
		std::string &newGazeboRunPy();

		const std::string &newGazeboFileName() const;
		std::string &newGazeboFileName();

		//string_vector_t allEngineProcEnvParams() const override;
		string_vector_t allEngineProcStartParams() const override;
};

using NewGazeboConfigSharedPtr = NewGazeboConfig::shared_ptr;
using NewGazeboConfigConstSharedPtr = NewGazeboConfig::const_shared_ptr;

#endif // NEW_GAZEBO_CONFIG_H

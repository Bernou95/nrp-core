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

#ifndef ENGINE_REQUEST_UTILS_H
#define ENGINE_REQUEST_UTILS_H

#include <string>
#include <nlohmann/json.hpp>

namespace EngineRequestUtils
{
    /*!
    * \brief Prepares JSON data for certain requests
    * \param requestData Pointer to the final data for the request
    * \param clientData Client data that should be included in the final data
    * \param config Configuration data. If left empty it will not be included in the final data
    */
    void prepareRequestData(      nlohmann::json * requestData,
                            const nlohmann::json & clientData,
                            const nlohmann::json & config = nlohmann::json());


    /*!
     * \brief Retrieves client data from the provided request JSON data
     * \param requestData JSON object from which the client data will be retrieved
     */
    const nlohmann::json & getClientDataFromRequest(const nlohmann::json & requestData);


    /*!
     * \brief Retrieves configuration data from the provided request JSON data
     * \param requestData JSON object from which the configuration data will be retrieved
     */
    const nlohmann::json & getConfigFromRequest(const nlohmann::json & requestData);
}

#endif // ENGINE_REQUEST_UTILS_H

// EOF

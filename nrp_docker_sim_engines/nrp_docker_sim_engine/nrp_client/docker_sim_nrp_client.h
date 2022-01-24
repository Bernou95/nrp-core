
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

#ifndef DOCKER_SIM_NRP_CLIENT_H
#define DOCKER_SIM_NRP_CLIENT_H

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_json_engine_protocol/nrp_client/engine_json_nrp_client.h"
#include "nrp_json_engine_protocol/config/engine_json_config.h"

#include "nrp_docker_sim_engine/config/docker_config.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <chrono>
#include <unistd.h>

#include "Python.h"
#define PYENGINE_MODULE "nrp_core.engines.docker_sim"
/*!
 * \brief NRP - Python Communicator on the NRP side. Converts DataPackInterface classes from/to JSON objects
 */
template<class ENGINE, const char* SCHEMA>
class DockerSimNrpClient
        : public EngineJSONNRPClient<ENGINE, SCHEMA>
{
    public:

        DockerSimNrpClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
                : EngineJSONNRPClient<ENGINE, SCHEMA>(config, std::move(launcher))
        {
            Py_Initialize();
            
            PyObject* pModule = PyImport_ImportModule(PYENGINE_MODULE);
            //PyObject* pDict = PyModule_GetDict(pModule);
            PyObject* pClass = PyObject_GetAttrString(pModule, "NetConnector");

            std::string configStr = this->engineConfig().dump();
            PyObject* pArg = PyTuple_New(1);                              
            PyTuple_SetItem(pArg, 0, Py_BuildValue("s", configStr.c_str()));
            PyObject* pObject = PyEval_CallObject(pClass, pArg);

            this->pLoop = PyObject_GetAttrString(pObject, "run_step");
            this->pReset = PyObject_GetAttrString(pObject, "reset");
            this->pShutdown = PyObject_GetAttrString(pObject, "shutdown");
        }

        virtual ~DockerSimNrpClient() override
        {
            Py_Finalize();
        }

        virtual void initialize() override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
        }

        virtual void reset() override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
            PyEval_CallObject(this->pReset, NULL);
            std::cout << "reset\n";
        }

        virtual void shutdown() override
        {   
            std::cout << "Docker Engine is shutting down !!!\n";
            for(int i = 0; i < 1000; i++){
                if(checkFlag == 1)  break;
                usleep(5);
            }
            PyEval_CallObject(this->pShutdown, NULL);
        }

        virtual const std::vector<std::string> engineProcStartParams() const override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
            std::vector<std::string> startParams;
            // Add Server address
            startParams.push_back("--host");
            startParams.push_back(this->engineConfig().at("IPHost"));
            startParams.push_back("--port");
            int port = this->engineConfig().at("Port");
            startParams.push_back(std::to_string(port));
            return startParams;
        }

        std::string toRunStep(std::string actionList){
            checkFlag = 0;
            PyObject* pAction = PyTuple_New(1);                               
            PyTuple_SetItem(pAction, 0, Py_BuildValue("s", actionList.c_str()));
            PyObject* pRet1 = PyEval_CallObject(this->pLoop, pAction);
            checkFlag = 1;
            char* info1=0;
            PyArg_Parse(pRet1, "s", &info1);
            std::string tmp_string(info1);
            return tmp_string;
        }

    private:
        /*!
         * \brief Error message returned by init command
         */
        std::string _initErrMsg = "";
        PyObject* pLoop = NULL;
        PyObject* pReset = NULL;
        PyObject* pShutdown = NULL;
        int checkFlag = 0;
};

#endif // DOCKER_SIM_NRP_CLIENT_H

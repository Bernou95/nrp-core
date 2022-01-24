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

#include "nrp_general_library/process_launchers/launch_commands/docker_fork.h"

#include "nrp_general_library/utils/nrp_exceptions.h"

#include <chrono>
#include <exception>
#include <iostream>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


DockerFork::~DockerFork()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Stop engine process if it's still running
    this->stopEngineProcess(60);
}

pid_t DockerFork::launchEngineProcess(const nlohmann::json &engineConfig, const std::vector<std::string> &envParams,
                                     const std::vector<std::string> &startParams, bool appendParentEnv)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    // Setup signal that closes process if parent process quits
    if(const auto prSig = prctl(PR_SET_PDEATHSIG, SIGHUP) < 0)
    {
        // Force quit if signal can't be created (Don't use the logger here, as this is a separate process)
        std::cerr << "Couldn't create parent kill signal. Error Code: " << prSig << "\nExiting...\n";
        std::cerr.flush();
        exit(prSig);
    }

    // Clear environment if it shouldn't be passed to engine
    if(!appendParentEnv)
        clearenv();

    std::vector<const char*> startParamPtrs;
    std::string startParamStr;

    // Reserve variable space (EnvCfgCmd + ENV_VAR1=ENV_VAL1 + ENV_VAR2=ENV_VAL2 + ... + engineProcCmd + --param1 + --param2 + ... + nullptr)
    startParamPtrs.reserve(envParams.size() + startParams.size() + 3);

    // Engine Exec cmd
    Py_Initialize();
    PyObject* pModule = PyImport_ImportModule(PY_CORE_MODULE);
    PyObject* pClass = PyObject_GetAttrString(pModule, "DockerConnector");
    std::string configStr = engineConfig.dump();
    PyObject* pArg = PyTuple_New(1);                              
    PyTuple_SetItem(pArg, 0, Py_BuildValue("s", configStr.c_str()));
    PyObject* pObject = PyEval_CallObject(pClass, pArg);

    this->pFunc = PyObject_GetAttrString(pObject, "getIpAdd");
    this->pShutdown = PyObject_GetAttrString(pObject, "shutdown");

    PyObject* pRet1 = PyEval_CallObject(this->pFunc, NULL);
    char* ipInfo=0;
    PyArg_Parse(pRet1, "s", &ipInfo);
    std::string tmp_string(ipInfo);
    this->hostIP = tmp_string;

    // Parameter end
    const auto tPid = getpid();
    this->_enginePID = tPid;
    startParamPtrs.push_back(nullptr);

    NRPLogger::info("Engine type: {}, name: {}, PID: {}", engineConfig.at("EngineType"), engineConfig.at("EngineName"), tPid);
    return tPid;
       

    return -1;      // Not executed, only here to prevent compiler warning
}

pid_t DockerFork::stopEngineProcess(unsigned int killWait)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    if(this->_enginePID > 0)
    {
        usleep(10*1000);
        PyEval_CallObject(this->pShutdown, NULL);
        // Set to maximum wait time if time is set to 0
        if(killWait == 0)
            killWait = std::numeric_limits<unsigned int>::max();
        // Check Nest status. After _killWait seconds, send SIGKILL to force a shutdown

        this->_enginePID = -1;
    }
    return 0;
}

LaunchCommandInterface::ENGINE_RUNNING_STATUS DockerFork::getProcessStatus()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Check if engine was already stopped before
    if(this->_enginePID < 0)
        return ENGINE_RUNNING_STATUS::RUNNING;

    // Check if this process received a stop notification for this child PID
    int engineStatus;
    if(waitpid(this->_enginePID, &engineStatus, WNOHANG | WUNTRACED) == this->_enginePID)
    {
        this->_enginePID = -1;
        return ENGINE_RUNNING_STATUS::STOPPED;
    }
    else
        return ENGINE_RUNNING_STATUS::RUNNING;
}


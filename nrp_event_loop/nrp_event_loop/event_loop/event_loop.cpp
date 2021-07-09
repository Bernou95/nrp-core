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

#include "nrp_event_loop/event_loop/event_loop.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

#include "nrp_event_loop/utils/graph_utils.h"

EventLoop::EventLoop(const nlohmann::json &config, std::chrono::milliseconds timestep, bool ownGIL) :
    _timestep(timestep),
    _ownGIL(ownGIL)
{
    if(!_ownGIL)
        _pyGILState = PyGILState_Ensure();

    boost::python::dict globalDict;
    createPythonGraphFromConfig(config, globalDict);

    if(!_ownGIL)
        PyGILState_Release(_pyGILState);
}

EventLoop::~EventLoop()
{
    this->shutdownLoop();
}

void EventLoop::runLoop()
{
    NRPLogger::debug("in loop");

    _doRun = true;

    while(_doRun) {
        auto now = std::chrono::steady_clock::now();

        if(!_ownGIL)
            _pyGILState = PyGILState_Ensure();

        ComputationalGraphManager::getInstance().compute();


        if(!_ownGIL)
            PyGILState_Release(_pyGILState);

        std::this_thread::sleep_until(now + _timestep);
    }

    NRPLogger::debug("out loop");
}

void EventLoop::runLoopAsync()
{ _loop_thread.reset(new std::thread(&EventLoop::runLoop, this)); }

void EventLoop::shutdownLoop()
{
    if(!_loop_thread)
        return;

    _doRun = false;

    try {
        _loop_thread->join();
    }
    catch(std::system_error& e) {
        NRPLogger::debug("Event Loop thread was not joinable because: {}, this is normal", e.what());
    }
}

ComputationalNode* EventLoop::getInputNode(const std::string& id)
{
    auto n = ComputationalGraphManager::getInstance().getNode(id);
    return n && n->type() == ComputationalNode::Input ? n : nullptr;
}

ComputationalNode* EventLoop::getOutputNode(const std::string& id)
{
    auto n = ComputationalGraphManager::getInstance().getNode(id);
    return n && n->type() == ComputationalNode::Output ? n : nullptr;
}

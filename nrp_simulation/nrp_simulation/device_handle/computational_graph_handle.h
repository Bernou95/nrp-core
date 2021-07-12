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

#ifndef COMPUTATION_GRAPH_HANDLE_H
#define COMPUTATION_GRAPH_HANDLE_H

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"
#include "nrp_event_loop/nodes/engine/input_node.h"
#include "nrp_event_loop/nodes/engine/output_node.h"

#include "nrp_simulation/device_handle/device_handle.h"

#include "nrp_event_loop/utils/graph_utils.h"

#include "nrp_general_library/utils/python_interpreter_state.h"

/*!
 * \brief Uses a Computation Graph to execute device transformation operations
 */
struct ComputationalGraphHandle : public DeviceHandle {

    ComputationalGraphHandle(bool slaveMode = false) :
        _slaveMode(slaveMode)
    {}

    bool _slaveMode;
    PyGILState_STATE _pyGILState;

    /*!
     * \brief Map containing all InputEngineNodes associated with this simulation
     */
    std::map<std::string, InputEngineNode*> _inputs;

    /*!
     * \brief Map containing all OutputEngineNodes associated with this simulation
     */
    std::map<std::string, OutputEngineNode*> _outputs;

    void init(const jsonSharedPtr &simConfig, const engine_interfaces_t &engines) override
    {
        // Load Computation Graph
        if(!_slaveMode) {
            boost::python::dict globalDict;
            createPythonGraphFromConfig(*simConfig, globalDict);
        }

        ComputationalGraphManager& gm = ComputationalGraphManager::getInstance();

        // Find Engine nodes in graph
        for(const auto &engine : engines) {
            std::string input_id = engine->engineName()+"_input";
            std::string output_id = engine->engineName()+"_output";

            if(gm.getNode(input_id) && dynamic_cast<InputEngineNode *>(gm.getNode(input_id)))
                _inputs[engine->engineName()] = dynamic_cast<InputEngineNode *>(gm.getNode(input_id));

            if(gm.getNode(output_id) && dynamic_cast<OutputEngineNode *>(gm.getNode(output_id)))
                _outputs[engine->engineName()] = dynamic_cast<OutputEngineNode *>(gm.getNode(output_id));
        }
    }

    void updateDevicesFromEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) override
    {
        if(_slaveMode)
            _pyGILState = PyGILState_Ensure();

        try
        {
            for(auto &engine : engines)
                if(_inputs.contains(engine->engineName())) {
                    _inputs[engine->engineName()]->setDevices(
                            engine->getDevicesFromEngine(_inputs[engine->engineName()]->requestedDevices()));
                }
        }
        catch(std::exception &)
        {
            // TODO: Handle failure on device retrieval
            throw;
        }

        if(_slaveMode)
            PyGILState_Release(_pyGILState);
    }

    void compute(const std::vector<EngineClientInterfaceSharedPtr> &/*engines*/) override
    {
        if(!_slaveMode)
            ComputationalGraphManager::getInstance().compute();
    }

    void sendDevicesToEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) override
    {
        if(_slaveMode)
            _pyGILState = PyGILState_Ensure();

        for(const auto &engine : engines)
            if(_outputs.contains(engine->engineName())) {
                try {
                    auto devs = _outputs[engine->engineName()]->getDevices();
                    engine->sendDevicesToEngine(devs);
                    for(auto d : devs)
                        delete d;
                }
                catch (std::exception &e) {
                    throw NRPException::logCreate(e,
                                                  "Failed to send devices to engine \"" + engine->engineName() + "\"");
                }
            }

        if(_slaveMode)
            PyGILState_Release(_pyGILState);
    }
};

#endif // COMPUTATION_GRAPH_HANDLE_H

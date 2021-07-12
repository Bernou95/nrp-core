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

#ifndef OUTPUT_ENGINE_NODE_H
#define OUTPUT_ENGINE_NODE_H

#include <boost/python.hpp>
#include <iostream>
#include <mutex>

#include "nrp_general_library/device_interface/device_interface.h"

#include "nrp_event_loop/computational_graph/output_node.h"
#include "nrp_event_loop/python/output_edge.h"

/*!
 * \brief Output node used to connect the computation graph with an EngineClient
 */
class OutputEngineNode : public OutputNode<boost::python::object> {
public:

    // TODO: make the node of type DeviceInterface and let DataConverter take care of bpy::extract

    /*!
     * Constructor
     */
    OutputEngineNode(const std::string &id) :
            OutputNode(id)
    { }

    /*!
     * Returns all devices stored in the node
     */
    std::vector<DeviceInterface*> getDevices()
    {
        std::lock_guard<std::mutex> lock(_devs_m);

        std::vector<DeviceInterface*> devs;
        for(auto &[id, dev] : _devices)
            devs.push_back(dev);

        _devices.clear();
        return devs;
    }

protected:

    // TODO: linking two ports to the same device will result in data overwriting and undefined behavior
    //  because node execution order is not in principled deterministic

    void sendOutputCallback(const std::string& /*id*/, const boost::python::object* data) override
    {
        std::lock_guard<std::mutex> lock(_devs_m);

        DeviceInterface* dev = boost::python::extract<DeviceInterface*>(*data);
        // Here device is copy to prevent graph corruption and to ensure the pointer is valid after future graph cycles
        _devices[dev->name()] = dev->clone();
    }

private:

    std::mutex _devs_m;
    std::map<std::string, DeviceInterface*> _devices;

};

class OutputEngineEdge : public SimpleOutputEdge<boost::python::object, OutputEngineNode> {

public:

    OutputEngineEdge(const std::string &keyword, const std::string &address) :
            SimpleOutputEdge(keyword, extractAddress(address).first, extractAddress(address).second)
    {}

protected:

    OutputEngineNode* makeNewNode() override
    { return new OutputEngineNode(this->_id); }

private:

    std::pair<std::string, std::string> extractAddress(const std::string& address)
    {
        auto n = address.find('/',1);
        auto name = address.substr(1, n-1);
        auto property = address.substr(n+1, address.size());

        return std::make_pair(name+"_output", property);
    }
};


#endif //OUTPUT_ENGINE_NODE_H

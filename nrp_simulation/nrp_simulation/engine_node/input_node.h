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

#ifndef INPUT_ENGINE_NODE_H
#define INPUT_ENGINE_NODE_H

#include <boost/python.hpp>
#include <mutex>

#include "nrp_general_library/device_interface/device_interface.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/input_edge.h"

/*!
 * \brief Input node used to connect an EngineClient with the computation graph
 *
 * It has two methods 'requestedDevices' and 'setDevices' which allows to update the node externally with the latest
 * devices
 */
class InputEngineNode : public InputNode<DeviceInterface> {
public:

    /*!
     * Constructor
     */
    InputEngineNode(const std::string &id) :
            InputNode(id, UpdatePolicy::LAST)
    {}

    /*!
     * Set devices that will be published into the graph in the next call to 'compute'
     */
    void setDevices(EngineClientInterface::devices_set_t devs)
    {
        std::lock_guard<std::mutex> lock(_devs_m);

        // move devices into temporary storage without copying shared pointer
        while(!devs.empty()) {
            auto n = devs.extract(devs.begin());
            if(this->_port_map.contains(n.value()->name()) &&
               (!_dev_temp.contains(n.value()->name()) || !n.value()->isEmpty()))
                _dev_temp[n.value()->name()] = std::move(n.value());
        }
    }

    /*!
     * Get the set of DeviceIdentifiers that this node requests
     */
    const std::set<DeviceIdentifier>& requestedDevices() const
    { return _dev_ids; }

    void configure() override
    {
        for(auto& [id, port] : _port_map)
            _dev_ids.insert(DeviceIdentifier(id, this->id().substr(0,this->id().find("_")), ""));
    }

protected:

    bool updateInputCallback(const std::string& id) override
    {
        std::lock_guard<std::mutex> lock(_devs_m);

        // move temp device to store without copying shared pointer and update pointer
        auto d = _dev_temp.find(id);
        if(d != _dev_temp.end() && (!_dev_store.contains(id) || !_dev_temp.at(id)->isEmpty())) {
            _dev_store[id] = std::move(d->second);
            _dev_temp.erase(d);
            _port_map.at(id).clear();
            _port_map.at(id).addMsg(_dev_store[id].get());
            return true;
        }

        return false;
    }

private:
    std::mutex _devs_m;

    std::map<std::string, DeviceInterfaceConstSharedPtr> _dev_temp;
    std::map<std::string, DeviceInterfaceConstSharedPtr> _dev_store;

    std::set<DeviceIdentifier> _dev_ids;
};


class InputEngineEdge : public SimpleInputEdge<DeviceInterface, InputEngineNode> {

public:

    InputEngineEdge(const std::string& keyword, const std::string& address) :
            SimpleInputEdge(keyword, extractAddress(address).first, extractAddress(address).second)
    {}

protected:

    InputEngineNode* makeNewNode() override
    { return new InputEngineNode(this->_id); }

private:

    std::pair<std::string, std::string> extractAddress(const std::string& address)
    {
        auto n = address.find('/',1);
        auto name = address.substr(1, n-1);
        auto property = address.substr(n+1, address.size());

        return std::make_pair(name+"_input", property);
    }
};


#endif //INPUT_ENGINE_NODE_H

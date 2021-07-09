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

#ifndef INPUT_NODE_H
#define INPUT_NODE_H

#include <memory>
#include <map>
#include <iostream>

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/output_port.h"

/*!
 * \brief Helper structure managing data and ports associated with a port id
 */
template<class DATA>
struct DataPortHandle {

    DataPortHandle() = delete;

    /*!
     * \brief Constructor
     */
    DataPortHandle(const std::string& id, ComputationalNode* parent, size_t queue_size) :
            single_port(new OutputPort<DATA>(id, parent)),
            list_port(new OutputPort<std::vector<const DATA*>>(id, parent))
    { data.reserve(queue_size); }

    /*!
     * \brief Publish last item in data
     */
    void publishLast()
    {
        if(data.size())
            single_port->publish(data.back());
    }

    /*!
     * \brief Publish all items in data
     */
    void publishAll()
    {
        if(data.size())
            list_port->publish(&data);
    }

    /*!
     * \brief Add a new message to the stored data
     */
    void addMsg(const DATA* msg)
    {
        if(data.size() < data.capacity())
            data.push_back(msg);
        else
            // TODO: change with proper info msg
            std::cout << "Port handle reached max size" << std::endl;
    }

    /*!
     * \brief Clear data
     */
    void clear()
    { data.clear(); }

    /*!
     * \brief Return the size of stored data
     */
    size_t size()
    { return data.size(); }

    /*! \brief Port used to send a single msg */
    std::shared_ptr<OutputPort<DATA>> single_port;
    /*! \brief Port used to send a list of msgs */
    std::shared_ptr<OutputPort<std::vector<const DATA*>>> list_port;

private:

    /*! \brief Data stored in the node. It is guaranteed to be static between 'compute' calls */
    std::vector<const DATA*> data;
};

/*!
 * \brief Implementation of an input node in the computation graph
 *
 * Input nodes are the connection points to feed data into the computation graph from outside. It owns output ports
 * which is use to forward incoming data to other nodes. One node can handle multiple ports
 *
 * Devired classes of InputNode remains responsible for the ownership of the data passed to InputNode through
 * updateInputCallback virtual method
 */
template <class DATA>
class InputNode : public ComputationalNode {
public:

    // TODO: Data stored by the node is de-/allocated in each compute call. Also, how Derived class stored temporary
    //  data between 'compute' calls is unspecified. InputNode could take a queue max size for _data to avoid dynamic
    //  allocation during compute state. Then derived classes can directly put new data in the allocated space

    /*! \brief Defines how this node send stored msgs */
    enum UpdatePolicy {
        LAST, /*!< only sends the last msg received  */
        ALL /*!< sends all msgs received since last 'compute' call  */
    };

    /*!
     * \brief Constructor
     */
    InputNode(const std::string &id, UpdatePolicy policy = UpdatePolicy::LAST, size_t queue_size = 10) :
        ComputationalNode(id, ComputationalNode::Input),
        _policy(policy),
        _queue_size(queue_size)
    { }

    /*!
     * \brief Configure
     */
    void configure() override
    {}

    /*!
     * \brief Compute. Updates and sends stored msgs.
     */
    void compute() override
    {
        // TODO: this loop could be possibly parallelized
        for(auto& [id, port]: _port_map) {
            if(!this->updateInputCallback(id))
                continue;
            else if (_policy == UpdatePolicy::LAST)
                port.publishLast();
            else if (_policy == UpdatePolicy::ALL)
                port.publishAll();
        }
    }

    /*!
     * \brief Registers an Output port with id 'id' with this node
     */
    void registerInput(const std::string& id)
    {
        if(!_port_map.contains(id))
            _port_map.emplace(id, DataPortHandle<DATA>(id, this, _queue_size));
    }

    /*!
     * \brief Returns a pointer to single port
     */
    OutputPort<DATA>* getSinglePort(const std::string& id)
    { return _port_map.contains(id) ? _port_map.at(id).single_port.get() : nullptr; }

    /*!
     * \brief Returns a pointer to list port
     */
    OutputPort<std::vector<DATA*>>* getListPort(const std::string& id)
    { return _port_map.contains(id) ? _port_map.at(id).list_port.get() : nullptr; }

    UpdatePolicy policy()
    { return _policy; }

protected:

    /*!
     * \brief After this function returns it is ensured that port 'id' is updated with the latest data
     *
     * Data pointers in the port are ensured to be valid until next call to this function.
     */
    virtual bool updateInputCallback(const std::string& id) = 0;

    /*! \brief Send policy used in this node */
    UpdatePolicy _policy;
    /*! \brief Map containing data to handle topics */
    std::map<std::string, DataPortHandle<DATA>> _port_map;
    /*! \brief Maximum number of msgs that the node can store per port */
    size_t _queue_size;
};


#endif //INPUT_NODE_H

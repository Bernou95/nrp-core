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

#ifndef OUTPUT_NODE_H
#define OUTPUT_NODE_H

#include <map>

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/input_port.h"

/*!
 * \brief Implementation of an output node in the computation graph
 *
 * Output nodes are the connection point to send data out of the computation graph. In its 'compute' operation all messages
 * received since last 'compute' are sent using the 'sendOutputCallback' implemented by derived classes.
 */
template <class DATA>
class OutputNode : public ComputationalNode {
public:

    /*!
     * \brief Constructor
     */
    OutputNode(const std::string &id)
        : ComputationalNode(id, ComputationalNode::Output)
    { }

    /*!
     * \brief Configure
     */
    void configure() override
    {
        // Allocate space for _msgs. It is assumed that no new ports are registered nor existing ones are subscribed after this call
        for(auto& [port_id, port]: _input_ports)
            _msgs[port_id].reserve(port->subscriptionsSize());
    }

    /*!
     * \brief Compute. It sends stored msgs out and clear the list
     */
    virtual void compute() override
    {
        // TODO: this loop could be possibly parallelized
        for (auto &[id, v] : _msgs)
            sendMsgs(id);
    }

    /*!
     * \brief Gets or register input port to this node and returns a pointer to it
     */
    template <class IN_T>
    InputPort<IN_T, DATA>* getOrRegisterInput(const std::string& id)
    {
        InputPort<IN_T, DATA>* port = nullptr;

        if(_input_ports.contains(id)) {
            port = dynamic_cast<InputPort<IN_T, DATA>*>(_input_ports.at(id).get());
            if(!port)
                throw NRPException::logCreate("In Output node " + this->id() + ", attempt to retrieve port " + id +
                    "with input data type" + typeid(IN_T).name() + ", but port was already registered with a different data type");
        }
        else {
            using std::placeholders::_1;
            std::function<void(const IN_T *)> f = std::bind(&OutputNode::storeMsg, this, id, _1);
            port = new InputPort<IN_T, DATA>(id, this, f);
            _input_ports.emplace(id, std::shared_ptr<Port>(port));
        }

        return port;
    }

protected:

    /*!
     * \brief Stores incoming msgs
     */
    void storeMsg(const std::string& id,const DATA* data)
    { _msgs[id].push_back(data); }

    void sendMsgs(const std::string& id)
    {
        for (auto m : _msgs[id])
            sendOutputCallback(id, m);

        _msgs[id].clear();
    }

    /*!
     * \brief Callback to send a msg to be implemented by derived classes
     */
    virtual void sendOutputCallback(const std::string& id, const DATA* data) = 0;

    /*! \brief List of ports owned by this node */
    std::map< std::string, std::shared_ptr<Port> > _input_ports;
    /*! \brief List of msgs stored*/
    std::map< std::string, std::vector<const DATA*>> _msgs;
};


#endif //OUTPUT_NODE_H

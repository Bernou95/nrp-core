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

#ifndef INPUT_EDGE_H
#define INPUT_EDGE_H

#include <boost/python.hpp>

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/functional_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

template<class T, class T_IN>
concept INPUT_C = std::is_base_of_v<InputNode<T_IN>, T>;

/*!
 * \brief Template which creates and registers an input node with UpdatePolicy = LAST
 *
 * Meant to be used in python input edge decorators
 */
template <class T_IN, INPUT_C<T_IN> INPUT_CLASS>
class SimpleInputEdge {

public:

    SimpleInputEdge() = delete;

    /*!
     * \brief Constructor
     */
    SimpleInputEdge(std::string keyword, std::string id, std::string port) :
            _keyword(std::move(keyword)), _id(std::move(id)), _port(std::move(port))
    {}

    /*!
     * \brief Setup edge from python
     */
    boost::python::object pySetup(const boost::python::object& obj)
    {
        // Register input node
        std::shared_ptr<ComputationalNode> node(makeNewNode());
        ComputationalGraphManager::getInstance().registerNode(node);
        INPUT_CLASS* i_node = dynamic_cast<INPUT_CLASS*>(node.get());
        if(!i_node)
            throw NRPException::logCreate("When creating InputEdge: node with the same name (\"+node->id()+\") already registered with a different type");

        assert(i_node->policy() == INPUT_CLASS::UpdatePolicy::LAST);
        i_node->registerInput(_port);

        // Register edge
        std::shared_ptr<PythonFunctionalNode> f = boost::python::extract< std::shared_ptr<PythonFunctionalNode> >(obj);
        ComputationalGraphManager::getInstance().registerEdge< T_IN,  boost::python::object >(
                i_node->getSinglePort(_port), f->getOrRegisterInput<T_IN>(_keyword));

        // Returns FunctionalNode
        return obj;
    }

protected:

    std::string _keyword;
    std::string _id;
    std::string _port;

    virtual INPUT_CLASS* makeNewNode() = 0;
};

#endif //INPUT_EDGE_H

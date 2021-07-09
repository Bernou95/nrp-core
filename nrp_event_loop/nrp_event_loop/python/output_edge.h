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

#ifndef OUTPUT_EDGE_H
#define OUTPUT_EDGE_H

#include "nrp_event_loop/computational_graph/output_node.h"
#include "nrp_event_loop/python/functional_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"


// dataConverter specialization
template <class T_OUT>
struct dataConverter<bpy::object, T_OUT> {
    static void convert(const bpy::object  *d1, T_OUT &d2)
    { d2 = boost::python::extract<T_OUT>(*d1); }
};

template<class T, class T_OUT>
concept OUTPUT_C = std::is_base_of_v<OutputNode<T_OUT>, T>;

/*!
 * \brief Template which creates and registers an output node with a single input port with the same id as the node
 *
 * Meant to be used in python output edge decorators
 */
template <class T_OUT, OUTPUT_C<T_OUT> OUTPUT_CLASS>
class SimpleOutputEdge {

public:

    SimpleOutputEdge() = delete;

    /*!
     * \brief Constructor
     */
    SimpleOutputEdge(std::string keyword, std::string id, std::string port) :
            _keyword(std::move(keyword)), _id(std::move(id)), _port(std::move(port))
    {}

    /*!
     * \brief Setup edge from python
     */
    boost::python::object pySetup(const boost::python::object& obj)
    {
        // Register output node
        std::shared_ptr<ComputationalNode> node(makeNewNode());
        ComputationalGraphManager::getInstance().registerNode(node);
        OUTPUT_CLASS* o_node = dynamic_cast<OUTPUT_CLASS*>(node.get());
        if(!o_node)
            throw NRPException::logCreate("When creating OutputEdge: node with the same name ("+node->id()+") already registered with a different type");

        // Register edge and input mapping
        InputPort<boost::python::object, T_OUT>* i_port = o_node->template getOrRegisterInput<boost::python::object>(_port);
        std::shared_ptr<PythonFunctionalNode> f = boost::python::extract< std::shared_ptr<PythonFunctionalNode> >(obj);
        ComputationalGraphManager::getInstance().registerEdge< boost::python::object,  T_OUT >(
                f->getOutputPort(_keyword), i_port);

        // Returns FunctionalNode
        return obj;
    }

protected:

    std::string _keyword;
    std::string _id;
    std::string _port;

    virtual OUTPUT_CLASS* makeNewNode() = 0;
};


#endif //OUTPUT_EDGE_H

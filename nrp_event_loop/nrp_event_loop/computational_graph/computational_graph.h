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

#ifndef COMPUTATION_GRAPH_H
#define COMPUTATION_GRAPH_H

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_event_loop/computational_graph/ngraph/ngraph.hpp"
#include "nrp_event_loop/computational_graph/computational_node.h"

/*!
 * /brief Class implementing a computation graph
 *
 * In the context of nrp-core a computation graph is defined as a directed, acyclic property graph in which nodes are associated
 * to objects of type ComputationalNode and which implements a 'compute' method. Nodes use Ports to communicate with each other.
 * Nodes are uniquely identified in the graph by an 'id' attribute which is of type string. Edges in the graph represent
 * connections between nodes, ie. between ports in the source and target nodes.
 *
 * Nodes can be of three types: 'Input', 'Output' and 'Functional'. 'Input' nodes can only be source nodes in edges.
 * 'Output' nodes can only be target nodes in edges. 'Functional' nodes can be both source and target.
 *
 * The graph itself implements a 'compute' function which calls 'compute' on all the nodes in the graph in the right order.
 * The latter is defined as follows: within a graph 'compute' operation, a node must always be executed after all the
 * source nodes in edges for which the former is target.
 *
 * The former definition on the execution order allows to divide the graph in layers which must be executed sequentially.
 * Nodes in each layer can be executed in parallel.
 *
 * The first layer will always be composed of nodes which have no inputs. These include 'Input' nodes and 'Functional'
 * with no inputs. For convenience, the latter are moved to the second layer (with no consequences) and the first
 * layer is kept with 'Input' nodes only.
 * In the same way, all 'Output' nodes are moved to a separate layer which is executed the last.
 */
class ComputationalGraph :
        private NGraph::tGraph<ComputationalNode *>
{
public:

    typedef std::vector<ComputationalGraph::vertex> comp_layer;

    /*!
     * \brief Insert edge
     */
    void insert_edge(const vertex &a, const vertex& b)
    {
        // TODO: should throw exception if graph configured or configuring. Find a solution for python FNEdge
        if(_isConfigured || _isConfiguring)
            std::cout << "Inserting edges while the graph is being configured or is configured can cause non defined behavior" << std::endl;

        // Enforce edge rules
        if(a->type() == ComputationalNode::Output)
            throw NRPException::logCreate("Attempt to insert edge with Output node \"" + a->id() + "\" as source. This is not allowed");
        if(b->type() == ComputationalNode::Input)
            throw NRPException::logCreate("Attempt to insert edge with Input node \"" + b->id() + "\" as target. This is not allowed");

        NGraph::tGraph<ComputationalNode *>::insert_edge(a, b);
    }

    /*!
     * \brief Clear graph
     */
    void clear()
    {
        clearLayers();
        NGraph::tGraph<ComputationalNode *>::clear();
        _isConfigured = false;
    }

    /*!
     * \brief Creates the graph execution structure and call 'configure' on each node
     */
    void configure()
    {
        if(_isConfigured)
            throw NRPException::logCreate("Graph is already configured. Please reset the graph first by calling 'clear()'");

        _isConfiguring = true;

        // Configure nodes
        for(const auto &e : *this)
            e.first->configure();

        // Clear layers
        clearLayers();

        // Creates input and output layers
        setIOLayers();

        // Creates other layers
        comp_layer layer;
        setFirstLayer(layer);
        while(!layer.empty()) {
            comp_layers.push_back(layer);
            layer = comp_layer();
            setNextLayer(comp_layers.back(), layer);
        }

        _isConfigured = true;
        _isConfiguring = false;
    }

    /*!
     * \brief Executes all nodes in the graph in order
     */
    void compute()
    {
        if(!_isConfigured || _isConfiguring)
            throw NRPException::logCreate("Graph is not configured. Please (re-)build the execution structure by calling 'configure()'");

        for(auto &node : input_layer)
            node->compute();

        for(auto &layer : comp_layers)
            for(auto &node : layer)
                node->compute();

        for(auto &node : output_layer)
            node->compute();
    }

    /*!
     * \brief Returns true if the graph is configured, false otherwise
     */
    bool isConfigured() const
    { return this->_isConfigured; }

private:

    /*!
     * \brief Clear graph structures
     */
    void clearLayers()
    {
        input_layer.clear();
        output_layer.clear();
        comp_layers.clear();
    }

    /*!
     * \brief Creates the first and last layers, composed by all 'Input' and 'Output' nodes respectively
     */
    void setIOLayers()
    {
        for(const auto &e : *this) {
            if(!e.first->isVisited() && e.first->type() == ComputationalNode::NodeType::Input) {
                e.first->setVisited(true);
                input_layer.push_back(e.first);
            }

            if(!e.first->isVisited() && e.first->type() == ComputationalNode::NodeType::Output) {
                e.first->setVisited(true);
                output_layer.push_back(e.first);
            }
        }
    }

    /*!
     * \brief Creates the first layer of the computation graph after the 'input layer'.
     *
     * Its elements are nodes of type 'Functional' with inputs of type 'Input' or no input.
     */
    void setFirstLayer(comp_layer& layer)
    {
        for(const auto &e : *this) {
            auto v = e.first;
            if(v->isVisited() || v->type() != ComputationalNode::NodeType::Functional)
                continue;

            bool isFirst = true;
            for(const auto &i : this->in_neighbors(v))
                if(i->type() != ComputationalNode::NodeType::Input) {
                    isFirst = false;
                    break;
                }

            if(isFirst) {
                v->setVisited(true);
                layer.push_back(v);
            }
        }
    }

    /*!
     * \brief given prev_layer finds the next layer in the computation graph
     *
     * the next layer is defined as the set of vertices with all inputs from prev_layer or layers previous to prev_layer
     */
    void setNextLayer(const comp_layer& prev_layer, comp_layer& layer)
    {
        // build output set
        ComputationalGraph::vertex_set output_set;
        for(const auto &v : prev_layer)
            output_set.insert(this->out_neighbors(v).begin(), this->out_neighbors(v).end());

        // build layer
        for(const auto &v : output_set) {
            if(v->type() == ComputationalNode::NodeType::Output)
                continue;

            if(v->type() == ComputationalNode::NodeType::Input || v->isVisited())
                throw NRPException::logCreate("Cycle detected in computation graph. Cycles are not allowed.");

            bool isNext = true;
            for(const auto &i : this->in_neighbors(v))
                if(i->type() != ComputationalNode::NodeType::Input && !i->isVisited()) {
                    isNext = false;
                    break;
                }

            if(isNext) {
                v->setVisited(true);
                layer.push_back(v);
            }
        }
    }

    comp_layer input_layer;
    comp_layer output_layer;
    std::vector<comp_layer> comp_layers;

    bool _isConfiguring = false;
    bool _isConfigured = false;
};


#endif //COMPUTATION_GRAPH_H

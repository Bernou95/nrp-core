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

#ifndef COMPUTATION_NODE_H
#define COMPUTATION_NODE_H

#include <string>

/*!
 * \brief Base class implementing a node in the computation graph
 */
class ComputationalNode {
public:

    /*! \brief All the possible node types */
    enum NodeType {
        Input, /*!< only can be source in edges  */
        Output, /*!< only can be target in edges  */
        Functional /*!< can be source and target, ie. can receive inputs and send outputs  */
    };

    ComputationalNode() = delete;

    /*!
     * \brief Constructor
     */
    ComputationalNode(std::string id, NodeType type) :
    _id(std::move(id)),
    _type(type)
    { }

    /*!
     * \brief Returns true if the node has been marked as visited, false otherwise
     */
    bool isVisited() const
    { return this->_visited; }

    /*!
     * \brief Sets a value for the node 'visited' property
     */
    void setVisited(bool visited)
    { this->_visited = visited; }

    /*!
     * \brief Returns the node 'id'
     */
    const std::string& id() const
    { return this->_id; }

    /*!
     * \brief Returns the node 'type'
     */
    NodeType type() const
    { return this->_type; }

    /*!
     * \brief Configures the node making it ready to execute 'compute'
     */
    virtual void configure() = 0;

    /*!
     * \brief Requests the node to execute its computation
     */
    virtual void compute() = 0;

    /*!
     * \brief Sets a value for the node 'configured' property
     */
    void setConfigured(bool configured)
    { this->_configured = configured; }

    /*!
     * \brief Returns true if the node is configured, false otherwise
     */
    bool isConfigured() const
    { return this->_configured; }

private:

    /*! \brief Node unique identifier */
    std::string _id;
    /*! \brief Node type */
    NodeType _type;
    /*! \brief Visited */
    bool _visited = false;
    /*! \brief Whether this node is in a configured state or not
     *
     * Each derived class can decide which events make the node un/configured */
    bool _configured = false;
};

#endif //COMPUTATION_NODE_H

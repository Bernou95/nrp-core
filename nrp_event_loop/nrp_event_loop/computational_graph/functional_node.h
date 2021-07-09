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

#ifndef FUNCTIONAL_NODE_H
#define FUNCTIONAL_NODE_H

#include <array>
#include <functional>
#include <tuple>

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/input_port.h"
#include "nrp_event_loop/computational_graph/output_port.h"


template<typename, typename>
class FunctionalNode;

/*!
 * \brief Implementation of a functional node in the computation graph
 *
 * It stores an std::function object, '_function' which is called in the node 'compute' method and which inputs and
 * outputs can be connected to input and output ports respectively
 */
template<typename... INPUT_TYPES, typename... OUTPUT_TYPES>
class FunctionalNode<std::tuple<INPUT_TYPES...>, std::tuple<OUTPUT_TYPES...> > : public ComputationalNode {

protected:

    /*! \brief Type of the nth element in INPUT_TYPES parameter pack */
    template <std::size_t N>
    using input_n_t = typename std::tuple_element<N, std::tuple<INPUT_TYPES...>>::type;

    /*! \brief Type of the nth element in OUTPUT_TYPES parameter pack */
    template <std::size_t N>
    using output_n_t = typename std::tuple_element<N, std::tuple<OUTPUT_TYPES...>>::type;

    using inputs_t = std::tuple<const INPUT_TYPES* ...>;
    using outputs_t = std::tuple<OUTPUT_TYPES ...>;
    
    /*! \brief Type representing the argument of _function. 
     * 
     * It is a tuple containing all input parameters as const pointers and all output as values
     */
    using params_t = decltype(std::tuple_cat(std::declval<inputs_t>(), std::declval<outputs_t>()));


public:

    /*! \brief Possible execution policies for this node */
    enum ExecutionPolicy {
        Always, /*!< the node is always executed when 'compute' is called */
        NewInput /*!< the node is executed only if at least one of its inputs have a fresh value */
    };

    /*!
     * \brief Configure
     */
    void configure() override
    {
        if(boundInputPorts() < std::tuple_size_v<inputs_t>)
            // TODO: replace with proper warning msg
            std::cout << "Functional node " << this->id() << " has been declared with " << std::tuple_size_v<inputs_t> <<
                    " inputs, but only " << boundInputPorts() << " are bounded" << std::endl;

        if(boundOutputPorts() < std::tuple_size_v<outputs_t>)
            // TODO: replace with proper warning msg
            std::cout << "Functional node " << this->id() << " has been declared with " << std::tuple_size_v<outputs_t> <<
                      " outputs, but only " << boundOutputPorts() << " are bounded" << std::endl;
    }

    /*!
     * \brief Compute. Execute '_function' and send its outputs
     */
    void compute() override
    {
        if(_policy == ExecutionPolicy::Always || _hasNew) {
            _function(_params);
            sendOutputs();
        }

        _hasNew = false;
    }

    template<size_t N = 0>
    void initInputs()
    {
        if constexpr ( N < sizeof...(INPUT_TYPES)) {
            std::get<N>(_params) = nullptr;
            initInputs<N+1>();
        }
    }

    /*!
     * \brief Process incoming msg
     */
    template <size_t N, class T>
    void newInputCallback(const T* value)
    {
        std::get<N>(_params) = value;
        this->_hasNew = true;
    }

    /*!
     * \brief Sends all outputs connected to ports
     */
    template<size_t N = 0>
    void sendOutputs()
    {
        if constexpr ( N < sizeof...(OUTPUT_TYPES)) {
            if (std::get<N>(_output_ports))
                std::get<N>(_output_ports)->publish(&std::get<sizeof...(INPUT_TYPES) + N>(_params));

            sendOutputs<N+1>();
        }
    }

    /*!
     * \brief Creates an InputPort and connect it to an input specified by N
     */
    template <std::size_t N, class T_IN, class T_OUT>
    InputPort<T_IN, T_OUT>* registerInput(const std::string& id)
    {
        if constexpr (N >= sizeof...(INPUT_TYPES)) {
            std::stringstream s;
            s << "In Functional node " << id << ". Registered input with index " << N << " greater than the number of inputs in this node " << sizeof...(INPUT_TYPES);
            throw NRPException::logCreate(s.str());
        }
        else if constexpr (!std::is_same_v<input_n_t<N>, T_OUT>) {
            std::stringstream s;
            s << "In Functional node " << id << ". Registered input with index " << N << " and type " << typeid(T_OUT).name() <<
                ", but index " << N << " is of type " << typeid(input_n_t<N>).name();
            throw NRPException::logCreate(s.str());
        }
        else if(_input_ports.at(N)) {
            std::stringstream s;
            s << "In Functional node " << id << ". Registered input with index " << N <<  " and name " << id <<
              ", but it was already registered with name " << _input_ports.at(N)->id();
            throw NRPException::logCreate(s.str());
        }

        using std::placeholders::_1;
        std::function<void(const T_OUT*)> f = std::bind( &FunctionalNode::newInputCallback<N, T_OUT>, this, _1 );
        InputPort<T_IN, T_OUT>* port = new InputPort<T_IN, T_OUT>(id, this, f, 1);
        _input_ports.at(N).reset(port);

        return port;
    }

    /*!
     * \brief Creates an OutputPort and connect it to an output specified by N
     */
    template <std::size_t N, class T>
    OutputPort<T>* registerOutput(const std::string& id)
    {
        if constexpr (N >= sizeof...(OUTPUT_TYPES)) {
            std::stringstream s;
            s << "In Functional node " << id << ". Registered output with index " << N << " greater than the number of outputs in this node " << sizeof...(OUTPUT_TYPES);
            throw NRPException::logCreate(s.str());
        }
        else if constexpr (!std::is_same_v<output_n_t<N>, T>) {
            std::stringstream s;
            s << "In Functional node " << id << ". Registered output with index " << N << " and type " << typeid(T).name() <<
              ", but index " << N << " is of type " << typeid(output_n_t<N>).name();
            throw NRPException::logCreate(s.str());
        }
        else if(std::get<N>(_output_ports)) {
            std::stringstream s;
            s << "In Functional node " << id << ". Registered output with index " << N <<  " and name " << id <<
              ", but it was already registered with name " << std::get<N>(_output_ports)->id();
            throw NRPException::logCreate(s.str());
        }

        OutputPort<T>* port = new OutputPort<T>(id, this);
        std::get<N>(_output_ports).reset(port);

        return port;
    }

protected:

    /*!
     * \brief Constructor
     */
    FunctionalNode(const std::string &id, std::function<void(params_t&)> f, ExecutionPolicy policy = ExecutionPolicy::NewInput) :
            ComputationalNode(id, ComputationalNode::Functional),
            _function(f),
            _policy(policy)
    { initInputs(); }

    size_t boundInputPorts()
    {
        size_t n = 0;
        for(auto p : _input_ports)
            n += p ? 1 : 0;

        return n;
    }

    template<size_t N = 0>
    size_t boundOutputPorts()
    {
        if constexpr (N < sizeof...(OUTPUT_TYPES)) {
            size_t n = std::get<N>(_output_ports) ? 1 : 0;
            return n + boundOutputPorts<N+1>();
        }

        return 0;
    }

    /*! \brief function performing main computation in this node */
    params_t _params;
    /*! \brief function performing main computation in this node. It sets the output part of _params from its input part */
    std::function<void(params_t&)> _function;

    /*! \brief input ports */
    std::array< std::shared_ptr<Port>, sizeof...(INPUT_TYPES) > _input_ports;
    /*! \brief output ports */
    std::tuple< std::shared_ptr< OutputPort<OUTPUT_TYPES> > ...> _output_ports;

    bool _hasNew = false;
    ExecutionPolicy _policy;

    friend class FunctionalNodeFactory;
};

#endif //FUNCTIONAL_NODE_H

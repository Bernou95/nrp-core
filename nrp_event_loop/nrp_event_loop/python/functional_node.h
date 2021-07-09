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

#ifndef PYTHON_FUNCTIONAL_NODE_H
#define PYTHON_FUNCTIONAL_NODE_H

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

#include "nrp_general_library/utils/nrp_exceptions.h"

#include "nrp_event_loop/computational_graph/functional_node.h"

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

namespace bpy = boost::python;

// dataConverter specialization
template <class T_IN>
struct dataConverter<T_IN, bpy::object> {
    static void convert(const T_IN *d1, bpy::object &d2)
    { d2 = bpy::object(bpy::ptr(d1)); }
};

// Helper struct to define a tuple type with N elements of type T
template <typename T, size_t N>
struct tuple_array {
    typedef decltype(std::tuple_cat(std::tuple<T>(), typename tuple_array<T, N-1>::type())) type;
};

template <typename T>
struct tuple_array<T, 0> {
    typedef decltype(std::tuple<>()) type;
};

/*!
 * \brief Specialization of FunctionalNode in which _function is a python callable object
 *
 * _function can have a variable number of inputs and outputs between 0 and input_s/output_s
 */
class PythonFunctionalNode : public FunctionalNode<typename tuple_array<bpy::object, 10>::type, typename tuple_array<bpy::object, 10>::type> {

    // TODO: publicly inheriting from FunctionalNode allows to bypass the safety checks when registering port in PythonFunctionalNode

    // TODO: PythonFunctionalNode should be a template with parameter size_t N = number of inputs and outputs. When doing this
    //  FunctionalNode template is not inferred properly. After figuring this out, instances of '3' should be replaced with N

    const static size_t input_s = 10;
    const static size_t output_s = 10;

public:

    /*!
     * \brief Constructor
     */
    PythonFunctionalNode(const std::string &id, const boost::python::list& o_ports) : 
        FunctionalNode(id, [] (params_t&) {})
    {
        bpy::stl_input_iterator<std::string> begin(o_ports), end;
        _o_port_ids.insert(_o_port_ids.begin(), begin, end);
    }

    /*!
     * \brief Set input pointers to point to their corresponding element in _params
     */
    template<std::size_t... Ints>
    void setInputPtrs(std::index_sequence<Ints...>)
    { ((_inputs[Ints] = &std::get<Ints>(_params)),...); }

    /*!
     * \brief Set output pointers to point to their corresponding element in _params
     */
    template<std::size_t... Ints>
    void setOutputPtrs(std::index_sequence<Ints...>)
    { ((_outputs[Ints] = &std::get<input_s+Ints>(_params)),...); }

    /*!
     * \brief Set python function arguments, calls it and set outputs
     */
    void callPython(params_t&)
    {
        boost::python::tuple args;
        boost::python::dict kwargs;

        for(size_t i=0; i < _i_port_ids.size(); ++i) {
            auto in = *_inputs[i];
            kwargs[_i_port_ids[i]] = in != nullptr ? *in : bpy::object();
        }

        // TODO: it is assumed that function returns a list with declared outputs, but it should be checked that this is true
        boost::python::list l_output = boost::python::extract<boost::python::list>(
                _python_f(*args, **kwargs));

        for(int i=0; i<boost::python::len(l_output); ++i)
            *_outputs[i] = l_output[i];
    }

    /*!
     * \brief Setup this node with a python callable object and registers it to ComputationalGraphManager
     */
    boost::python::object pySetup(boost::python::object obj)
    {
        // Create heap allocated shared pointer from 'this' and set it up
        std::shared_ptr<PythonFunctionalNode> self = this->moveToSharedPtr();
        self->_python_f = obj;
        self->_function = std::bind(&PythonFunctionalNode::callPython, self, std::placeholders::_1 );
        self->setInputPtrs(std::make_index_sequence<input_s>());
        self->setOutputPtrs(std::make_index_sequence<output_s>());

        // Collect function arguments
        auto inspect = bpy::import("inspect");
        bpy::stl_input_iterator<std::string> begin(inspect.attr("getfullargspec")(obj).attr("args")), end;
        self->_i_port_ids.insert(self->_i_port_ids.begin(), begin, end);

        // Setup ports
        if(self->_o_port_ids.size() > output_s || self->_i_port_ids.size() > output_s)
            throw NRPException::logCreate("PythonFunctionalNode has not enough input or output ports to wrap the "
                                          "requested python object");

        for(const auto& p : self->_o_port_ids)
            self->registerOutputPort(p);

        // Register node
        std::shared_ptr<ComputationalNode> self_base = std::dynamic_pointer_cast<ComputationalNode>(self);
        ComputationalGraphManager::getInstance().registerNode(self_base);

        return boost::python::object(self);
    }

    /*!
     * \brief Configure this node
     */
    void configure() override
    {
        // Create edges to other functional nodes
        for (auto& [port_id, address]: _fn_edges) {
            auto n = address.find('/',1);
            auto name = address.substr(1, n-1);
            auto property = address.substr(n+1, address.size());

            // Get ports
            PythonFunctionalNode* node = dynamic_cast<PythonFunctionalNode*>(ComputationalGraphManager::getInstance().getNode(name));
            OutputPort<bpy::object>* o_port = node->getOutputPort(property);
            InputPort<bpy::object, bpy::object>* i_port = this->getOrRegisterInput<bpy::object>(port_id);

            // Register edge
            ComputationalGraphManager::getInstance().registerEdge<bpy::object, bpy::object>(o_port, i_port);
        }

        // check unbound inputs and print warning
        for(size_t i=0; i < _i_port_ids.size(); ++i)
            if(!_input_ports[i])
                // TODO: replace with proper warning msg
                std::cout << "In python functional node \"" << this->id() << "\". Input argument \"" << _i_port_ids[i] <<
                          "\" is not connected" << std::endl;
    }

    /*!
     * \brief Safely get or register an input port
     */
    template<typename T_IN, size_t N = 0>
    InputPort<T_IN, bpy::object>* getOrRegisterInput(const std::string& id)
    {
        auto m_iter = std::find(_i_port_ids.begin(), _i_port_ids.end(), id);
        if(m_iter == _i_port_ids.end())
            throw NRPException::logCreate("\"" + id + "\" does not match any of the declared input arguments in node \"" + this->id() + "\"");

        if constexpr (N < input_s) {
            if(_input_ports[N]) {
                if(_input_ports[N]->id() == id)
                    return dynamic_cast< InputPort<T_IN, bpy::object>* >(_input_ports[N].get());
                else
                    return getOrRegisterInput<T_IN, N + 1>(id);
            }
            else {
                std::iter_swap(m_iter, _i_port_ids.begin() + N);
                return FunctionalNode::registerInput<N, T_IN, bpy::object>(id);
            }
        }

        throw NRPException::logCreate("There is no input port with name" + id + "registered to this node and no additional ports can be registered");
    }

    /*!
     * \brief Safely get an output port
     */
    template<size_t N = 0>
    OutputPort<bpy::object>* getOutputPort(const std::string& id)
    {
        if constexpr (N < output_s) {
            if(std::get<N>(_output_ports) && std::get<N>(_output_ports)->id() == id)
                return std::get<N>(_output_ports).get();
            else
                return getOutputPort<N+1>(id);
        }

        throw NRPException::logCreate("There is no output port with name \"" + id + "\" registered to this node");
    }

    /*!
     * \brief Request the registration of an edge between an output port in another functional node an i_port input port in this node
     */
    void registerFNEdge(const std::string& i_port, const std::string& address)
    { _fn_edges[i_port] = address; }

private:

    /*!
     * \brief Safely registers an output port
     */
    template<size_t N = 0>
    OutputPort<bpy::object>* registerOutputPort(const std::string& id)
    {
        if constexpr (N < output_s) {
            if(!std::get<N>(_output_ports))
                return FunctionalNode::registerOutput<N, bpy::object>(id);
            else
                return registerOutputPort<N+1>(id);
        }

        throw NRPException::logCreate("All output ports are registered already");
    }

    std::map<std::string, std::string> _fn_edges;
    std::vector<std::string> _i_port_ids;
    std::vector<std::string> _o_port_ids;

    /*! \brief helper function to move this to a shared pointer */
    std::shared_ptr<PythonFunctionalNode> moveToSharedPtr()
    { return std::shared_ptr<PythonFunctionalNode>(new PythonFunctionalNode(std::move(static_cast<PythonFunctionalNode&>(*this)))); }

    /*! \brief python callable object wrapped in this node */
    boost::python::object _python_f;

    /*! \brief array of pointers to the input part of _params. The pointers are set in setInputPtrs */
    std::array< const bpy::object**, input_s > _inputs;
    /*! \brief array of pointers to the output part of _params. The pointers are set in setOutputPtrs */
    std::array< bpy::object*, output_s > _outputs;
};

class InputFNEdge {
public:

    InputFNEdge(const std::string &keyword, const std::string &address) :
            _keyword(keyword),
            _address(address)
    {  }

    boost::python::object pySetup(const boost::python::object& obj)
    {
        // Register edge and input mapping
        std::shared_ptr<PythonFunctionalNode> f = boost::python::extract< std::shared_ptr<PythonFunctionalNode> >(obj);
        f->registerFNEdge(_keyword, _address);

        // Returns FunctionalNode
        return obj;
    }

protected:

    std::string _keyword;
    std::string _address;
};



#endif //PYTHON_FUNCTIONAL_NODE_H

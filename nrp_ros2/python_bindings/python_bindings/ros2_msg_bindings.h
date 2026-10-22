/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef NRP_ROS2_MSG_BINDINGS_H
#define NRP_ROS2_MSG_BINDINGS_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <array>
#include <string>
#include <vector>

/*!
 * \brief Helpers used by the generated boost python bindings of ROS 2 messages
 *
 * Container types (std::vector, std::array) are shared by many messages, possibly in different packages and python
 * modules. They are registered only once.
 */
namespace nrp_ros2
{
    namespace bpy = boost::python;

    /*!
     * \brief Whether a python class has already been registered for C++ type T
     */
    template<class T>
    bool isRegistered()
    {
        const bpy::converter::registration *reg = bpy::converter::registry::query(bpy::type_id<T>());
        return reg != nullptr && reg->m_to_python != nullptr;
    }

    /*!
     * \brief Converts python sequences (lists, tuples, bytes, numpy arrays, ...) to CONTAINER.
     * This allows to assign them directly to message fields, e.g. msg.data = [1, 2, 3]
     */
    template<class CONTAINER>
    struct SequenceConverter
    {
        using value_type = typename CONTAINER::value_type;

        static void registerConverter()
        { bpy::converter::registry::push_back(&convertible, &construct, bpy::type_id<CONTAINER>()); }

        static void *convertible(PyObject *obj)
        {
            if(!PySequence_Check(obj) || PyUnicode_Check(obj))
                return nullptr;

            // Objects which already wrap CONTAINER are converted by the class itself
            if(bpy::converter::get_lvalue_from_python(obj, bpy::converter::registered<CONTAINER>::converters))
                return nullptr;

            return obj;
        }

        static void construct(PyObject *obj, bpy::converter::rvalue_from_python_stage1_data *data)
        {
            void *storage = reinterpret_cast<bpy::converter::rvalue_from_python_storage<CONTAINER>*>(data)->storage.bytes;
            auto *container = new (storage) CONTAINER();

            bpy::object seq(bpy::handle<>(bpy::borrowed(obj)));
            const auto len = bpy::len(seq);
            fill(*container, seq, len);

            data->convertible = storage;
        }

    private:
        template<class T>
        static void fill(std::vector<T> &container, const bpy::object &seq, long len)
        {
            container.reserve(len);
            for(long i = 0; i < len; ++i)
                container.push_back(bpy::extract<T>(seq[i]));
        }

        template<class T, std::size_t N>
        static void fill(std::array<T, N> &container, const bpy::object &seq, long len)
        {
            if(len != static_cast<long>(N))
            {
                PyErr_SetString(PyExc_ValueError, ("Expected a sequence of length " + std::to_string(N) +
                                                   ", got one of length " + std::to_string(len)).c_str());
                bpy::throw_error_already_set();
            }

            for(long i = 0; i < len; ++i)
                container[i] = bpy::extract<T>(seq[i]);
        }
    };

    /*!
     * \brief Python sequence protocol for containers which can't use vector_indexing_suite (std::array and
     * std::vector<bool>). Iteration works through __getitem__
     */
    template<class CONTAINER>
    struct SequenceProtocol
    {
        using value_type = typename CONTAINER::value_type;

        static std::size_t index(const CONTAINER &c, long i)
        {
            if(i < 0)
                i += static_cast<long>(c.size());

            if(i < 0 || i >= static_cast<long>(c.size()))
            {
                PyErr_SetString(PyExc_IndexError, "Index out of range");
                bpy::throw_error_already_set();
            }

            return static_cast<std::size_t>(i);
        }

        static std::size_t len(const CONTAINER &c)
        {   return c.size();   }

        static value_type get(const CONTAINER &c, long i)
        {   return c[index(c, i)];  }

        static void set(CONTAINER &c, long i, const value_type &v)
        {   c[index(c, i)] = v; }

        static void append(CONTAINER &c, const value_type &v)
        {   c.push_back(v); }
    };

    /*!
     * \brief Registers std::vector<T> as python class NAME
     */
    template<class T>
    void exposeVector(const char *name)
    {
        if(isRegistered<std::vector<T>>())
            return;

        bpy::class_<std::vector<T>>(name)
                .def(bpy::vector_indexing_suite<std::vector<T>>());

        SequenceConverter<std::vector<T>>::registerConverter();
    }

    /*!
     * \brief Registers std::vector<bool> as python class NAME. vector_indexing_suite doesn't support it
     */
    template<>
    inline void exposeVector<bool>(const char *name)
    {
        using container_t = std::vector<bool>;
        using protocol_t = SequenceProtocol<container_t>;

        if(isRegistered<container_t>())
            return;

        bpy::class_<container_t>(name)
                .def("__len__", &protocol_t::len)
                .def("__getitem__", &protocol_t::get)
                .def("__setitem__", &protocol_t::set)
                .def("append", &protocol_t::append);

        SequenceConverter<container_t>::registerConverter();
    }

    /*!
     * \brief Registers std::array<T, N> as python class NAME
     */
    template<class T, std::size_t N>
    void exposeArray(const char *name)
    {
        using container_t = std::array<T, N>;
        using protocol_t = SequenceProtocol<container_t>;

        if(isRegistered<container_t>())
            return;

        bpy::class_<container_t>(name)
                .def("__len__", &protocol_t::len)
                .def("__getitem__", &protocol_t::get)
                .def("__setitem__", &protocol_t::set);

        SequenceConverter<container_t>::registerConverter();
    }

    /*!
     * \brief Message as YAML string, used for python __str__
     */
    template<class MSG>
    std::string toYaml(const MSG &msg)
    {   return to_yaml(msg);    }
}

#endif // NRP_ROS2_MSG_BINDINGS_H

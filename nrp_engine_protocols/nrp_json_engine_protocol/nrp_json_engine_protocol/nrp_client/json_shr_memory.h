//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#ifndef JSON_SHARED_MEMORY_H
#define JSON_SHARED_MEMORY_H

#include <nlohmann/json.hpp>
#include "nrp_json_engine_protocol/datapack_interfaces/json_datapack.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;

#define MAX_KEY_NAME 20
struct KeyType {
    char name[MAX_KEY_NAME];
    nlohmann::json::value_t type = nlohmann::json::value_t::null;
    nlohmann::json::value_t arrayType;
    bool isSet = true;
};

typedef allocator<KeyType, managed_shared_memory::segment_manager>  KeyAllocator;
typedef vector<KeyType, KeyAllocator> KeyVector;

namespace json_shared_memory
{
//// construct

    void datapackShrConstructObj(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data, KeyVector *keys);
    void datapackShrWriteObj(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data, KeyVector *keys);
    void datapackShrReadObj(managed_shared_memory& segment, const std::string& name, KeyVector *keys, nlohmann::json* data);

    template <class T>
    void datapackShrConstructArray(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data)
    {
        // Construct vector storing the array
        typedef allocator<T, managed_shared_memory::segment_manager>  TAllocator;
        typedef vector<T, TAllocator> TVector;

        const TAllocator alloc_inst (segment.get_segment_manager());
        TVector *vecShr = segment.construct<TVector>(name.c_str())(alloc_inst);

        // Copy data to vector
        // TODO: it is assumed that all items in the array have the same type, but this should be checked for safety
        for(auto& el : data) {
            try {
                if(!el.is_null())
                    vecShr->push_back(el.get<T>());
                else
                    vecShr->push_back(0);
            }
            catch (const nlohmann::json::exception& e) {
                NRPLogger::info("failed to construct " + name + " error: " + e.what());
            }
        }
    }

    template <>
    inline void datapackShrConstructArray<nlohmann::json>(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data)
    {
        // Construct vector storing the array
        typedef allocator<KeyVector, managed_shared_memory::segment_manager>  TAllocator;
        typedef vector<KeyVector, TAllocator> TVector;

        const TAllocator alloc_inst (segment.get_segment_manager());
        TVector *vecShr = segment.construct<TVector>(name.c_str())(alloc_inst);

        for(size_t i = 0; i < data.size(); ++i) {
            const KeyAllocator key_alloc (segment.get_segment_manager());
            KeyVector keys(key_alloc);
            datapackShrConstructObj(segment, name + "." + std::to_string(i), data[i], &keys);
            vecShr->push_back(keys);
        }
    }

    inline nlohmann::json::value_t datapackShrConstructArrayFindType(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data)
    {
        if(data.empty() || !data.is_array())
            throw NRPException::logCreate("Attempt to store an empty json array or not array");

        // Get type of the first element, all elements are assumed to be of the same type
        switch (data[0].type()) {
            case nlohmann::json::value_t::boolean:
                datapackShrConstructArray<bool>(segment, name, data);
                break;
            case nlohmann::json::value_t::number_integer:
                datapackShrConstructArray<nlohmann::json::number_integer_t>(segment, name, data);
                break;
            case nlohmann::json::value_t::number_unsigned:
                datapackShrConstructArray<nlohmann::json::number_unsigned_t>(segment, name, data);
                break;
            case nlohmann::json::value_t::number_float:
                datapackShrConstructArray<double>(segment, name, data);
                break;
            case nlohmann::json::value_t::object:
                datapackShrConstructArray<nlohmann::json>(segment, name, data);
                break;
            default:
                return  nlohmann::json::value_t::null;
//                throw NRPException::logCreate("Array type " +
//                                              std::to_string(static_cast<int>(data[0].type())) +
//                                              " not supported: " + name + " / " + data.dump());
        }

        return data[0].type();
    }

    inline void datapackShrConstructObj(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data, KeyVector *keys)
    {
        // construct values
        // TODO: it should be checked that data is of type object
        for(auto& el : data.items()) {
            // Add key to keys
            KeyType key;
            strcpy(key.name, el.key().c_str()); // TODO: check that key fits in array
            key.type = el.value().type();
            std::string valName = name + "." + el.key();

            // construct value
            bool addKey = true;
            switch (el.value().type()) {
                case nlohmann::json::value_t::array:
                    key.arrayType = datapackShrConstructArrayFindType(segment, valName, el.value());
                    if(key.arrayType == nlohmann::json::value_t::null) {
                        addKey = false;
                        NRPLogger::info("datapack " + name + " field " + valName + ". Ignoring because type is unsupported: " + el.value().dump());
                    }

                    break;
                case nlohmann::json::value_t::boolean:
                    segment.construct<bool>(valName.c_str())(el.value().get<bool>());
                    break;
                case nlohmann::json::value_t::number_integer:
                    segment.construct<nlohmann::json::number_integer_t>(valName.c_str())(el.value().get<nlohmann::json::number_integer_t>());
                    break;
                case nlohmann::json::value_t::number_unsigned:
                    segment.construct<nlohmann::json::number_unsigned_t>(valName.c_str())(el.value().get<nlohmann::json::number_unsigned_t>());
                    break;
                case nlohmann::json::value_t::number_float:
                    segment.construct<double>(valName.c_str())(el.value().get<double>());
                    break;
                default: // object, string, discarded, null
                    addKey = false;
                    NRPLogger::info("datapack " + name + " field " + valName + ". Ignoring because type is unsupported: " + el.value().dump());
//                    throw NRPException::logCreate("nlohmann::json type " +
//                                                  std::to_string(static_cast<int>(el.value().type())) +
//                                                  " not supported: " + name + " / " + data.dump());
            }

            if(addKey)
                keys->push_back(key);
        }
    }

//// write

    template <class T>
    void datapackShrWriteArray(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data)
    {
        // Get vector storing the array
        typedef allocator<T, managed_shared_memory::segment_manager>  TAllocator;
        typedef vector<T, TAllocator> TVector;
        TVector *vecShr = segment.find<TVector>(name.c_str()).first;
        if (!vecShr)
            throw NRPException::logCreate("Error when writing array to shared memory: " + name + " not found");

        // Overwrite content
        vecShr->clear();
        for(auto& el : data) {
            try {
                if(!el.is_null())
                    vecShr->push_back(el.get<T>());
                else
                    vecShr->push_back(0);
            }
            catch (const nlohmann::json::exception& e) {
                NRPLogger::info("failed to write " + name + " error: " + e.what());
            }
        }
    }

    template <>
    inline void datapackShrWriteArray<nlohmann::json>(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data)
    {
        // Construct vector storing the array
        typedef allocator<KeyVector, managed_shared_memory::segment_manager>  TAllocator;
        typedef vector<KeyVector, TAllocator> TVector;
        TVector *vecShr = segment.find<TVector>(name.c_str()).first;
        if (!vecShr)
            throw NRPException::logCreate("Error when writing array to shared memory: " + name + " not found");
        if (vecShr->size() != data.size())
            throw NRPException::logCreate("Attempt to write Array of objects with a different size. Name: " +
            name + " old size: " + std::to_string(vecShr->size()) + " new size: " + std::to_string(data.size()));

        // TODO: unsafe, assuming objects are stored in the same order as when construct, not guaranteed and error prompt
        for(size_t i = 0; i < data.size(); ++i)
            datapackShrWriteObj(segment, name + "." + std::to_string(i), data[i], &((*vecShr)[i]));
    }

    inline nlohmann::json::value_t datapackShrWriteArrayFindType(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data)
    {
        if(data.empty() || !data.is_array())
            throw NRPException::logCreate("Attempt to store an empty json array or not array");

        // Get type of the first element, all elements are assumed to be of the same type
        switch (data[0].type()) {
            case nlohmann::json::value_t::boolean:
                datapackShrWriteArray<bool>(segment, name, data);
                break;
            case nlohmann::json::value_t::number_integer:
                datapackShrWriteArray<nlohmann::json::number_integer_t>(segment, name, data);
                break;
            case nlohmann::json::value_t::number_unsigned:
                datapackShrWriteArray<nlohmann::json::number_unsigned_t>(segment, name, data);
                break;
            case nlohmann::json::value_t::number_float:
                datapackShrWriteArray<double>(segment, name, data);
                break;
            case nlohmann::json::value_t::object:
                datapackShrWriteArray<nlohmann::json>(segment, name, data);
                break;
            default:
                return  nlohmann::json::value_t::null;
//                throw NRPException::logCreate("Array type " +
//                                              std::to_string(static_cast<int>(data[0].type())) +
//                                              " not supported: " + name + " / " + data.dump());
        }

        return data[0].type();
    }

    template<class T>
    void datapackShrSetValue(managed_shared_memory& segment, const std::string& name, const T& data)
    {
        T *val = segment.find<T>(name.c_str()).first;
        if (!val)
            throw NRPException::logCreate("Error when writing key to shared memory: " + name + " not found");
        else
            *val = data;
    }

    inline void datapackShrWriteObj(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data, KeyVector *keys)
    {
        // TODO: if data contains keys which were not stored for this datapack when it was constructed (datapackShrConstructObj)
        //  they will be lost
        for(auto& key : *keys) {
            if(data.contains(key.name)) {
                std::string valName = name + "." + key.name;
                auto val = data.at(key.name);
                key.isSet = true;
                switch (val.type()) {
                    case nlohmann::json::value_t::array:
                        if(datapackShrWriteArrayFindType(segment, valName, val) == nlohmann::json::value_t::null)
                            key.isSet = false;
                        break;
                    case nlohmann::json::value_t::boolean:
                        datapackShrSetValue<bool>(segment, valName, val.get<bool>());
                        break;
                    case nlohmann::json::value_t::number_integer:
                        datapackShrSetValue<nlohmann::json::number_integer_t>(segment, valName, val.get<nlohmann::json::number_integer_t>());
                        break;
                    case nlohmann::json::value_t::number_unsigned:
                        datapackShrSetValue<nlohmann::json::number_unsigned_t>(segment, valName, val.get<nlohmann::json::number_unsigned_t>());
                        break;
                    case nlohmann::json::value_t::number_float:
                        datapackShrSetValue<double>(segment, valName, val.get<double>());
                        break;
                    default: // object, string, discarded, null
                        // just ignore the field if it is not supported
                        key.isSet = false;
//                        throw NRPException::logCreate("nlohmann::json type " +
//                                                      std::to_string(static_cast<int>(val.type())) +
//                                                      " not supported: " + name + " / " + data.dump());
                }
            }
            else
                key.isSet = false;
        }
    }

//// read

    template <class T>
    void  datapackShrReadArray(managed_shared_memory& segment, const std::string& name, nlohmann::json* data)
    {
        typedef allocator<T, managed_shared_memory::segment_manager>  TAllocator;
        typedef vector<T, TAllocator> TVector;
        TVector *vecShr = segment.find<TVector>(name.c_str()).first;
        if (!vecShr)
            throw NRPException::logCreate("Error when reading array from shared memory: " + name + " not found");

        for(const auto& el : *vecShr)
            data->push_back(el);
    }

    template <>
    inline void datapackShrReadArray<nlohmann::json>(managed_shared_memory& segment, const std::string& name, nlohmann::json* data)
    {
        // Construct vector storing the array
        typedef allocator<KeyVector, managed_shared_memory::segment_manager>  TAllocator;
        typedef vector<KeyVector, TAllocator> TVector;
        TVector *vecShr = segment.find<TVector>(name.c_str()).first;
        if (!vecShr)
            throw NRPException::logCreate("Error when reading array from shared memory: " + name + " not found");

        // TODO: unsafe, assuming objects are stored in the same order as when construct, not guaranteed and error prompt
        for(size_t i = 0; i < vecShr->size(); ++i) {
            data->push_back(nlohmann::json());
            datapackShrReadObj(segment, name + "." + std::to_string(i), &((*vecShr)[i]), &((*data)[i]));
        }
    }

    inline void datapackShrReadArrayFindType(managed_shared_memory& segment, const std::string& name, nlohmann::json::value_t arrayType, nlohmann::json* data)
    {
        switch (arrayType) {
            case nlohmann::json::value_t::boolean:
                datapackShrReadArray<bool>(segment, name, data);
                break;
            case nlohmann::json::value_t::number_integer:
                datapackShrReadArray<nlohmann::json::number_integer_t>(segment, name, data);
                break;
            case nlohmann::json::value_t::number_unsigned:
                datapackShrReadArray<nlohmann::json::number_unsigned_t>(segment, name, data);
                break;
            case nlohmann::json::value_t::number_float:
                datapackShrReadArray<double>(segment, name, data);
                break;
            case nlohmann::json::value_t::object:
                datapackShrReadArray<nlohmann::json>(segment, name, data);
                break;
            default:
                // should never reach here really
                throw NRPException::logCreate("Array type " +
                                              std::to_string(static_cast<int>(arrayType)) +
                                              " not supported");
        }
    }

    template<class T>
    T* datapackShrReadValue(managed_shared_memory& segment, const std::string& name)
    {
        T *val = segment.find<T>(name.c_str()).first;
        if (!val)
            throw NRPException::logCreate("Error when reading key from shared memory: " + name + " not found");

        return val;
    }

    inline void datapackShrReadObj(managed_shared_memory& segment, const std::string& name, KeyVector *keys, nlohmann::json* data)
    {
        // overwrite values
        for(auto& key : *keys) {
            if(key.isSet) {
                std::string valName = name + "." + key.name;
                switch (key.type) {
                    case nlohmann::json::value_t::array: {
                        nlohmann::json arrayData;
                        datapackShrReadArrayFindType(segment, valName, key.arrayType, &arrayData);
                        (*data)[key.name] = arrayData;
                        break;
                    }
                    case nlohmann::json::value_t::boolean:
                        (*data)[key.name] = *(datapackShrReadValue<bool>(segment, valName));
                        break;
                    case nlohmann::json::value_t::number_integer:
                        (*data)[key.name] = *(datapackShrReadValue<nlohmann::json::number_integer_t>(segment, valName));
                        break;
                    case nlohmann::json::value_t::number_unsigned:
                        (*data)[key.name] = *(datapackShrReadValue<nlohmann::json::number_unsigned_t>(segment,
                                                                                                      valName));
                        break;
                    case nlohmann::json::value_t::number_float:
                        (*data)[key.name] = *(datapackShrReadValue<double>(segment, valName));
                        break;
                    // should never reach here really
                    default: // object, string, discarded, null
                        throw NRPException::logCreate("nlohmann::json type " +
                                                      std::to_string(static_cast<int>(key.type)) +
                                                      " not supported");
                }
            }
        }
    }

//// main accessors

// Writes or construct a datapack to shared memory
    inline void datapackShrConstructOrWrite(managed_shared_memory& segment, const std::string& name, const nlohmann::json& data)
    {
        bool doConstruct = false;
        KeyVector *vecShr = segment.find<KeyVector>(name.c_str()).first;
        if(!segment.find<KeyVector>(name.c_str()).first) {
            const KeyAllocator alloc_inst (segment.get_segment_manager());
            vecShr = segment.construct<KeyVector>(name.c_str())(alloc_inst);
            doConstruct = true;
        }

//        NRPLogger::info("Writing: " + name + ", construct: " + std::to_string(doConstruct));

        switch (data.type()) {
            case nlohmann::json::value_t::object:
                if (doConstruct)
                    datapackShrConstructObj(segment, name, data, vecShr);
                else
                    datapackShrWriteObj(segment, name, data, vecShr);
                break;
            case nlohmann::json::value_t::array:
                // if it is an array with a single object, just remove the array
//                if (data.size() == 1 && data[0].type() == nlohmann::json::value_t::object) {
//                    if (doConstruct)
//                        datapackShrConstructObj(segment, name, data[0], vecShr);
//                    else
//                        datapackShrWriteObj(segment, name, data[0], vecShr);
//                }
//                else {
                    if (doConstruct) {
                        KeyType k;
                        k.type = nlohmann::json::value_t::null;
                        // TODO: handle the null type case (which is return if not supported type is found)
                        k.arrayType = datapackShrConstructArrayFindType(segment, name+"_array", data);
                        vecShr->push_back(k);
                    }
                    else
                        datapackShrWriteArrayFindType(segment, name+"_array", data);
//                }
                break;
            default:
                throw NRPException::logCreate("nlohmann::json datapack data must be object or array");
        }
    }

// Reads a datapack from shared memory
    inline nlohmann::json* datapackShrRead(managed_shared_memory& segment, const std::string& datapackName)
    {
        KeyVector *vecShr = segment.find<KeyVector>(datapackName.c_str()).first;
        if(!vecShr)
            throw NRPException::logCreate("Can't read datapack " + datapackName + ". Can't be found in shared memory");

        nlohmann::json* data = new nlohmann::json();

//        NRPLogger::info("Reading: " + datapackName);

        // array
        if(vecShr->size() == 1 && vecShr->at(0).type == nlohmann::json::value_t::null)
            datapackShrReadArrayFindType(segment, datapackName+"_array", vecShr->at(0).arrayType, data);
            // object
        else
            datapackShrReadObj(segment, datapackName, vecShr, data);

        return data;
    }

} // namespace json_shared_memory

#endif

// EOF

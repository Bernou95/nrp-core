/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020 Michael Zechmair
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

#ifndef DUMMY_DEVICE_H
#define DUMMY_DEVICE_H

#include "nrp_general_library/device_interface/device.h"

// Helper structure, for cleaner code

struct DummyDevicePropSpec
{
    static constexpr FixedString Scalar        = "scalar"; // Property name
    static constexpr float       DefaultScalar = 0.0f;     // Default value

    using DummyArray = std::array<float, 2>;

    static constexpr FixedString Array         = "array";
    // Do not use auto here!
    static constexpr DummyArray  DefaultArray  = { 0.0f, 0.0f };

    static constexpr FixedString String        = "string";
    static constexpr auto        DefaultString = "";

    using DummyVector = std::vector<int>;

    static constexpr FixedString Vector        = "vector"; // No default value here, because it can't be a compile time constant

    // Aggregate property names into single structure PropNames
    // This will be passed to PropertyTemplates constructor

    using DummyDevicePropNames = PropNames<Scalar, Array, String, Vector>;
};

// Actual device class
// Derives from Device, which in turn derives from PropertyTemplates
// Also derives from DummyDevicePropSpec for convenience

class DummyDevice
    : public DummyDevicePropSpec,
      // Here we need to pass property names and specify property types
      public Device<DummyDevice, "DummyDevice", DummyDevicePropSpec::DummyDevicePropNames, float, DummyDevicePropSpec::DummyArray, std::string, DummyDevicePropSpec::DummyVector>
{
    public:
        DummyDevice(DeviceIdentifier    &&devId,
                    property_template_t &&props = property_template_t(DefaultScalar, DefaultArray, DefaultString, DummyVector()))
            : Device(std::move(devId), std::move(props))
        {
            // Do nothing
        }

        // Getter and setter for "Scalar" property

        float getScalar() const
        {
            return getPropertyByName<Scalar>();
        }

        void setScalar(float value)
        {
            getPropertyByName<Scalar>() = value;
        }

        // Getter and setter for "Array" property

        const DummyArray & getArray() const
        {
            return getPropertyByName<Array>();
        }

        void setArray(const DummyArray & value)
        {
            getPropertyByName<Array>() = value;
        }

        // Getter and setter for "String" property

        const std::string & getString() const
        {
            return getPropertyByName<String>();
        }

        void setString(const std::string & value)
        {
            getPropertyByName<String>() = value;
        }

        // Getter and setter for "Vector" property

        const DummyVector & getVector() const
        {
            return getPropertyByName<Vector>();
        }

        void setVector(const DummyVector & value)
        {
            getPropertyByName<Vector>() = value;
        }
};

#endif // DUMMY_DEVICE_H
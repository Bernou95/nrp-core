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

#ifndef DEVICE_H
#define DEVICE_H

#include "nrp_general_library/device_interface/device_interface.h"
#include "nrp_general_library/device_interface/device_serializer_methods.h"

/*!
 * \brief Device class. All devices must inherit from this one
 * \tparam DEVICE Final derived device class
 * \tparam TYPE Device Type
 * \tparam PROP_NAMES Property Names
 * \tparam PROPERTIES Device Properties
 */
template<class DEVICE, FixedString TYPE, PROP_NAMES_C PROP_NAMES, class ...PROPERTIES>
class Device
        : public DeviceInterface,
          public PropertyTemplate<DEVICE, PROP_NAMES, PROPERTIES...>,
          public PtrTemplates<DEVICE>
{
	public:
		static constexpr FixedString TypeName = TYPE;
		using property_template_t = typename PropertyTemplate<DEVICE, PROP_NAMES, PROPERTIES...>::property_template_t;

		using shared_ptr = typename PtrTemplates<DEVICE>::shared_ptr;
		using const_shared_ptr = typename PtrTemplates<DEVICE>::const_shared_ptr;
		using unique_ptr = typename PtrTemplates<DEVICE>::unique_ptr;
		using const_unique_ptr = typename PtrTemplates<DEVICE>::const_unique_ptr;

		virtual ~Device() override = default;

		/*!
		 * \brief Constructor
		 * \tparam DEV_ID_T DeviceIdentifier type
		 * \tparam PROPERTIES_T Property types to pass along to PropertyTemplate constructor
		 * \param devID Device ID
		 * \param props Properties to pass along to PropertyTemplate constructor
		 */
		template<class DEV_ID_T, class ...PROPERTIES_T>
		requires(std::same_as<std::remove_cvref_t<DEV_ID_T>, DeviceIdentifier>)
		Device(DEV_ID_T &&devID, PROPERTIES_T &&...props)
		    : DeviceInterface(std::forward<DEV_ID_T>(devID)),
		      property_template_t(std::forward<PROPERTIES_T>(props)...)
		{
			// Make sure DEVICE class is derived from DeviceInterface
			static_assert(DEVICE_C<DEVICE>, "DEVICE does not fulfill concept requirements");

			this->setIsEmpty(false);
		};

		template<class STRING1_T, class STRING2_T>
		requires(std::constructible_from<std::string, STRING1_T> && std::constructible_from<std::string, STRING2_T>)
		static DeviceIdentifier createID(STRING1_T &&name, STRING2_T &&engineName)
		{	return DeviceIdentifier(std::forward<STRING1_T>(name), std::forward<STRING2_T>(engineName), DEVICE::TypeName);	}

		/*!
		 * \brief Deserialize property data into PropertyTemplate format
		 * \tparam DESERIALZER_T Type to deserialize
		 * \tparam PROPERTIES_T Type of default properties
		 * \param data Property Data to deserialize
		 * \param props Default values. Used if data does not initialize a certain value
		 * \return Returns PropertyTemplate of this device
		 */
		template<class DESERIALIZER_T, class ...PROPERTIES_T>
		static property_template_t deserializeProperties(DESERIALIZER_T &&data, PROPERTIES_T &&...props)
		{
			return PropertySerializer<std::remove_cvref_t<DESERIALIZER_T>, DEVICE>::template deserializeProperties(std::forward<DESERIALIZER_T>(data),
			                                                                                                std::forward<PROPERTIES_T>(props)...);
		}

		virtual DeviceInterfaceConstSharedPtr moveToSharedPtr() override final
		{
			return this->moveToSharedPtrHelper();
		}

	private:
		
		typename PtrTemplates<DEVICE>::const_shared_ptr moveToSharedPtrHelper() const
		{	
			return typename PtrTemplates<DEVICE>::const_shared_ptr(new DEVICE(std::move(static_cast<const DEVICE&>(*this))));
		}
};

#endif // DEVICE_H

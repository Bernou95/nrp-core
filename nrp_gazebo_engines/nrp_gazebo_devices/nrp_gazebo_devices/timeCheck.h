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

#ifndef TIMECHECK_H
#define TIMECHECK_H

#include "nrp_general_library/device_interface/device.h"
#include "nrp_general_library/property_template/serializers/json_property_serializer.h"

class TimeCheck;

struct TimeCheckConst
{
	static constexpr FixedString Timeval = "timeval";
	static constexpr float DefTimeval = 0.0;
	using JPropNames = PropNames<Timeval>;
};

class TimeCheck
        : public TimeCheckConst,
          public Device<TimeCheck, "TimeCheck", TimeCheckConst::JPropNames, float>
{
	public:
		TimeCheck(DeviceIdentifier &&devID, property_template_t &&props = property_template_t(DefTimeval))
		    : Device(std::move(devID), std::move(props))
		{}

		template<class DESERIALIZE_T>
		static auto deserializeProperties(DESERIALIZE_T &&data){			
			return Device::deserializeProperties(
			std::forward<DESERIALIZE_T>(data), DefTimeval);	}

		float timeval() const;
		void setTimeval(float timeVal);

};

template<>
nlohmann::json JSONPropertySerializerMethods::serializeSingleProperty(const float &property);

template<>
float JSONPropertySerializerMethods::deserializeSingleProperty(const nlohmann::json &data, const std::string_view &name);


#endif // TIMECHECK_H

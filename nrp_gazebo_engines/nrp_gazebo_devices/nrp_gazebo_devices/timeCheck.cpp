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

#include "nrp_gazebo_devices/timeCheck.h"

float TimeCheck::timeval() const
{
	return this->getPropertyByName<Timeval>();
}

void TimeCheck::setTimeval(float timeval)
{
	this->getPropertyByName<Timeval>() = timeval;
}


template<>
nlohmann::json JSONPropertySerializerMethods::serializeSingleProperty(const float &property)
{
	return nlohmann::json(property);
}

template<>
float JSONPropertySerializerMethods::deserializeSingleProperty(const nlohmann::json &data, const std::string_view &name)
{
	const auto dataIterator(data.find(name.data()));
	if(dataIterator != data.end())
	{
		if(dataIterator->is_number())
			return dataIterator->get<float>();
		else
			return NAN;
	}
	else
		throw NRPExceptionMissingProperty(std::string("Couldn't find JSON attribute \"") + name.data() + "\" during deserialization");
}
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

#ifndef NEST_PY_JSON_DEVICE_CONTROLLER_H
#define NEST_PY_JSON_DEVICE_CONTROLLER_H

#include "nrp_python_device/devices/pyobject_device.h"
#include "nrp_json_engine_protocol/device_interfaces/json_device_serializer.h"
#include "nrp_json_engine_protocol/engine_server/engine_json_device_controller.h"
#include "nrp_general_library/property_template/serializers/python_dict_property_serializer.h"

#include "nrp_nest_json_engine/devices/nest_device.h"
#include <boost/python.hpp>

template<class DEVICE>
class NestPyJSONDeviceController;

template<>
class NestPyJSONDeviceController<NestDevice>
        : public EngineDeviceController<nlohmann::json, NestDevice>
{
	public:
		NestPyJSONDeviceController(DeviceIdentifier &&devID, boost::python::object data = boost::python::object());

		virtual ~NestPyJSONDeviceController() override = default;

		virtual void handleDeviceDataCallback(NestDevice &&data) override;
		virtual const NestDevice *getDeviceInformationCallback() override;

		/*!
		 * \brief Get python object referenced by this controller
		 */
		boost::python::object &data();
		boost::python::object data() const;

	protected:
		/*!
		 * \brief Device Data. Used to convert to/from JSON and python dict
		 */
		NestDevice _deviceData;
};

#endif // NEST_PY_JSON_DEVICE_CONTROLLER_H

//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020 Michael Zechmair
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

#include "nrp_opensim_json_engine/devices/opensim_engine_json_device_controller.h"

#include <iostream>

namespace py = boost::python;

OpensimEngineJSONDeviceController<PyObjectDevice>::OpensimEngineJSONDeviceController(DeviceIdentifier &&devID, boost::python::object data)
    : EngineDeviceController<nlohmann::json, PyObjectDevice>(std::move(devID)),
      _deviceData(DeviceIdentifier(*this), data)
{}

void OpensimEngineJSONDeviceController<PyObjectDevice>::handleDeviceDataCallback(PyObjectDevice &&data)
{
	this->_deviceData = std::move(data);
}

const PyObjectDevice *OpensimEngineJSONDeviceController<PyObjectDevice>::getDeviceInformationCallback()
{	
	std::string tStr = py::extract<std::string>(py::str(this->_deviceData.data()))();
	//std::cout << "1 --> "<< tStr << std::endl;
	return &(this->_deviceData);
}

boost::python::object &OpensimEngineJSONDeviceController<PyObjectDevice>::data()
{
	return this->_deviceData.data();
}

boost::python::object OpensimEngineJSONDeviceController<PyObjectDevice>::data() const
{
	return this->_deviceData.data();
}

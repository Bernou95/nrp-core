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

#include <boost/python.hpp>

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_opensim_json_engine/config/cmake_constants.h"
#include "nrp_opensim_json_engine/engine_server/opensim_json_server.h"
#include "nrp_opensim_json_engine/python/opensim_engine_script.h"
#include "nrp_opensim_json_engine/python/opensim_engine_script_wrapper.h"


namespace python = boost::python;

/*!
 * \brief Calls PythonJSONServer::registerScript() without returning a value
 */
void OpensimServerRegistration(python::object script)
{	OpensimnJSONServer::registerScript(script);	}

/*!
 * \brief Decorator for engine script class.
 * User can decorate their EngineScript class with this to register it with the server
 */
struct OpensimRegisterEngineDecorator
{
	/*!
	 * \brief __call__() function
	 * \param script Class derived from EngineScript
	 * \return Returns ref to PyEngineScript
	 */
	OpensimEngineScript &pyCall(python::object script)
	{	return *(OpensimJSONServer::registerScript(script));	}
};


BOOST_PYTHON_MODULE(NRP_OPENSIM_ENGINE_MODULE)
{
	//python::class_<PyEngineScript>("__IntEngineScript", python::init<>());

	// Import General NRP Python Module
	python::import(PYTHON_MODULE_NAME_STR);
	python::import("opensim");


	python::class_<SimulationTime>("SimulationTime")
		.def("count", &SimulationTime::count)
	;

	// Engine Script Class. Used by users to define script that should be executed
	python::class_<OpensimEngineScriptWrapper, boost::noncopyable>("EngineScript", python::init<>())
	        .def("initialize", &OpensimEngineScriptWrapper::initialize, &OpensimEngineScriptWrapper::defaultInitialize)
	        .def("runLoop", python::pure_virtual(&OpensimEngineScriptWrapper::runLoop))
	        .def("shutdown", &OpensimEngineScriptWrapper::shutdown, &OpensimEngineScriptWrapper::defaultShutdown)
	        .add_property("_time", &OpensimEngineScript::simTime)
	        .def("_registerDevice", &OpensimEngineScript::registerDevice)
	        .def("_getDevice", &OpensimEngineScript::getDevice, python::return_value_policy<python::copy_non_const_reference>())
	        .def("_setDevice", &OpensimEngineScript::setDevice);

	python::register_ptr_to_python<OpensimEngineScriptSharedPtr>();
	python::register_ptr_to_python<OpensimEngineScriptConstSharedPtr>();

	// Engine Registration functions
	python::class_<OpensimRegisterEngineDecorator, boost::noncopyable>("RegisterEngine", python::init<>())
	        .def("__call__", &OpensimRegisterEngineDecorator::pyCall, python::return_internal_reference<>());
}

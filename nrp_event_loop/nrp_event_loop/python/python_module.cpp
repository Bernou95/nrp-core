#include <boost/python.hpp>

#include "nrp_event_loop/python/functional_node.h"
#include "nrp_event_loop/python/input_dummy.h"
#include "nrp_event_loop/python/output_dummy.h"

namespace bpy = boost::python;

BOOST_PYTHON_MODULE(NRPEventLoopEngine)
{

    bpy::class_<PythonFunctionalNode>("FunctionalNode", bpy::init<const std::string &, const boost::python::list&>( (bpy::arg("name"), bpy::arg("outputs")) ))
            .def("__call__", &PythonFunctionalNode::pySetup);

    bpy::class_<InputFNEdge>("inputFN", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &InputFNEdge::pySetup);

    bpy::class_< InputDummyEdge >("inputDummy", bpy::init<const std::string &, const std::string &, boost::python::object>((bpy::arg("keyword"), bpy::arg("address"), bpy::arg("value")) ))
            .def("__call__", &InputDummyEdge::pySetup);

    bpy::class_< OutputDummyEdge >("outputDummy", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &OutputDummyEdge::pySetup);

    bpy::register_ptr_to_python<std::shared_ptr<PythonFunctionalNode> >();
}
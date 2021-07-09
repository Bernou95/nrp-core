#include <boost/python.hpp>

#include "nrp_simulation/engine_node/input_node.h"
#include "nrp_simulation/engine_node/output_node.h"

namespace bpy = boost::python;

BOOST_PYTHON_MODULE(PyNRPSimulation)
{

    bpy::class_< InputEngineEdge >("FromEngine", bpy::init<const std::string &, const std::string &>((bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &InputEngineEdge::pySetup);

    bpy::class_< OutputEngineEdge >("ToEngine", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &OutputEngineEdge::pySetup);

}
#include <boost/python.hpp>
#include "encode.h"
using namespace boost::python;

namespace python = boost::python;

BOOST_PYTHON_MODULE(tClient){
  python::class_<GZ_Client>("GZ_Client", python::init<>())
  .def_readwrite("value", &GZ_Client::value)
  .add_property("_msg", &GZ_Client::getMsg)
  .add_property("_sim_time", &GZ_Client::getSimTime)
  .def("setListener", &GZ_Client::setListener)
  .def("toListen", &GZ_Client::toListen)
  .def("shutdown", &GZ_Client::shutdown);
  def("hello", hello);
}
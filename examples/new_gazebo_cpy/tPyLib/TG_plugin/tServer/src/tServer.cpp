#include <boost/python.hpp>
#include "encode.h"
using namespace boost::python;

namespace python = boost::python;

BOOST_PYTHON_MODULE(tServer){
  python::class_<GZ_Server>("GZ_Server", python::init<>())
  .def_readwrite("value", &GZ_Server::value)
  .def("setWorld", &GZ_Server::setWorld)
  .def("loadWorld", &GZ_Server::loadWorld)
  .def("shutdown", &GZ_Server::shutdown);
  def("hello", hello);
}

#include <geometry_msgs/Inertia.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Inertia& m1, const Inertia& m2)
{
  return false;
}

void exportInertia ()
{
  using geometry_msgs::Inertia;
  class_<Inertia, shared_ptr<Inertia> >("Inertia", "# Mass [kg]\nfloat64 m\n\n# Center of mass [m]\ngeometry_msgs/Vector3 com\n\n# Inertia Tensor [kg-m^2]\n#     | ixx ixy ixz |\n# I = | ixy iyy iyz |\n#     | ixz iyz izz |\nfloat64 ixx\nfloat64 ixy\nfloat64 ixz\nfloat64 iyy\nfloat64 iyz\nfloat64 izz\n")
    .def_readwrite("m", &Inertia::m)
    .def_readwrite("com", &Inertia::com)
    .def_readwrite("ixx", &Inertia::ixx)
    .def_readwrite("ixy", &Inertia::ixy)
    .def_readwrite("ixz", &Inertia::ixz)
    .def_readwrite("iyy", &Inertia::iyy)
    .def_readwrite("iyz", &Inertia::iyz)
    .def_readwrite("izz", &Inertia::izz)
    ;

}

} // namespace
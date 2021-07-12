
#include <geometry_msgs/Vector3.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Vector3& m1, const Vector3& m2)
{
  return false;
}

void exportVector3 ()
{
  using geometry_msgs::Vector3;
  class_<Vector3, shared_ptr<Vector3> >("Vector3", "# This represents a vector in free space. \n# It is only meant to represent a direction. Therefore, it does not\n# make sense to apply a translation to it (e.g., when applying a \n# generic rigid transformation to a Vector3, tf2 will only apply the\n# rotation). If you want your data to be translatable too, use the\n# geometry_msgs/Point message instead.\n\nfloat64 x\nfloat64 y\nfloat64 z")
    .def_readwrite("x", &Vector3::x)
    .def_readwrite("y", &Vector3::y)
    .def_readwrite("z", &Vector3::z)
    ;

}

} // namespace
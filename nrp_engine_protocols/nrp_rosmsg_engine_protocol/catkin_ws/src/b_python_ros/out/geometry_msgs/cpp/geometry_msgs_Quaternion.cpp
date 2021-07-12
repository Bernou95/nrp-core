
#include <geometry_msgs/Quaternion.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Quaternion& m1, const Quaternion& m2)
{
  return false;
}

void exportQuaternion ()
{
  using geometry_msgs::Quaternion;
  class_<Quaternion, shared_ptr<Quaternion> >("Quaternion", "# This represents an orientation in free space in quaternion form.\n\nfloat64 x\nfloat64 y\nfloat64 z\nfloat64 w\n")
    .def_readwrite("x", &Quaternion::x)
    .def_readwrite("y", &Quaternion::y)
    .def_readwrite("z", &Quaternion::z)
    .def_readwrite("w", &Quaternion::w)
    ;

}

} // namespace
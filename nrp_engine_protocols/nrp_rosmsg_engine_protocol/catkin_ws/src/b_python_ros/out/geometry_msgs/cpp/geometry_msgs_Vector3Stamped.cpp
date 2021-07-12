
#include <geometry_msgs/Vector3Stamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Vector3Stamped& m1, const Vector3Stamped& m2)
{
  return false;
}

void exportVector3Stamped ()
{
  using geometry_msgs::Vector3Stamped;
  class_<Vector3Stamped, shared_ptr<Vector3Stamped> >("Vector3Stamped", "# This represents a Vector3 with reference coordinate frame and timestamp\nHeader header\nVector3 vector\n")
    .def_readwrite("header", &Vector3Stamped::header)
    .def_readwrite("vector", &Vector3Stamped::vector)
    ;

}

} // namespace

#include <geometry_msgs/QuaternionStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const QuaternionStamped& m1, const QuaternionStamped& m2)
{
  return false;
}

void exportQuaternionStamped ()
{
  using geometry_msgs::QuaternionStamped;
  class_<QuaternionStamped, shared_ptr<QuaternionStamped> >("QuaternionStamped", "# This represents an orientation with reference coordinate frame and timestamp.\n\nHeader header\nQuaternion quaternion\n")
    .def_readwrite("header", &QuaternionStamped::header)
    .def_readwrite("quaternion", &QuaternionStamped::quaternion)
    ;

}

} // namespace
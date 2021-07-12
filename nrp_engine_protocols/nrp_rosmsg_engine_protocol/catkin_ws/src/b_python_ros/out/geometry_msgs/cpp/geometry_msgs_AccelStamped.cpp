
#include <geometry_msgs/AccelStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const AccelStamped& m1, const AccelStamped& m2)
{
  return false;
}

void exportAccelStamped ()
{
  using geometry_msgs::AccelStamped;
  class_<AccelStamped, shared_ptr<AccelStamped> >("AccelStamped", "# An accel with reference coordinate frame and timestamp\nHeader header\nAccel accel\n")
    .def_readwrite("header", &AccelStamped::header)
    .def_readwrite("accel", &AccelStamped::accel)
    ;

}

} // namespace
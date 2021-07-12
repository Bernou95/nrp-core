
#include <geometry_msgs/TwistStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const TwistStamped& m1, const TwistStamped& m2)
{
  return false;
}

void exportTwistStamped ()
{
  using geometry_msgs::TwistStamped;
  class_<TwistStamped, shared_ptr<TwistStamped> >("TwistStamped", "# A twist with reference coordinate frame and timestamp\nHeader header\nTwist twist\n")
    .def_readwrite("header", &TwistStamped::header)
    .def_readwrite("twist", &TwistStamped::twist)
    ;

}

} // namespace
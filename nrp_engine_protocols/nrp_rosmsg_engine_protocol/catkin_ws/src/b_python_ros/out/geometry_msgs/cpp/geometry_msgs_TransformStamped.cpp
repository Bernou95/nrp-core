
#include <geometry_msgs/TransformStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const TransformStamped& m1, const TransformStamped& m2)
{
  return false;
}

void exportTransformStamped ()
{
  using geometry_msgs::TransformStamped;
  class_<TransformStamped, shared_ptr<TransformStamped> >("TransformStamped", "# This expresses a transform from coordinate frame header.frame_id\n# to the coordinate frame child_frame_id\n#\n# This message is mostly used by the \n# <a href=\"http://wiki.ros.org/tf\">tf</a> package. \n# See its documentation for more information.\n\nHeader header\nstring child_frame_id # the frame id of the child frame\nTransform transform\n")
    .def_readwrite("header", &TransformStamped::header)
    .def_readwrite("child_frame_id", &TransformStamped::child_frame_id)
    .def_readwrite("transform", &TransformStamped::transform)
    ;

}

} // namespace
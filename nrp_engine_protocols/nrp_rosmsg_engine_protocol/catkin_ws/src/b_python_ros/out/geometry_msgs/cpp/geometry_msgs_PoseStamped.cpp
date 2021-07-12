
#include <geometry_msgs/PoseStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const PoseStamped& m1, const PoseStamped& m2)
{
  return false;
}

void exportPoseStamped ()
{
  using geometry_msgs::PoseStamped;
  class_<PoseStamped, shared_ptr<PoseStamped> >("PoseStamped", "# A Pose with reference coordinate frame and timestamp\nHeader header\nPose pose\n")
    .def_readwrite("header", &PoseStamped::header)
    .def_readwrite("pose", &PoseStamped::pose)
    ;

}

} // namespace
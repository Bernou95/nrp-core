
#include <geometry_msgs/Pose.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Pose& m1, const Pose& m2)
{
  return false;
}

void exportPose ()
{
  using geometry_msgs::Pose;
  class_<Pose, shared_ptr<Pose> >("Pose", "# A representation of pose in free space, composed of position and orientation. \nPoint position\nQuaternion orientation\n")
    .def_readwrite("position", &Pose::position)
    .def_readwrite("orientation", &Pose::orientation)
    ;

}

} // namespace
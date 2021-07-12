
#include <nav_msgs/Odometry.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace nav_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Odometry& m1, const Odometry& m2)
{
  return false;
}

void exportOdometry ()
{
  using nav_msgs::Odometry;
  class_<Odometry, shared_ptr<Odometry> >("Odometry", "# This represents an estimate of a position and velocity in free space.  \n# The pose in this message should be specified in the coordinate frame given by header.frame_id.\n# The twist in this message should be specified in the coordinate frame given by the child_frame_id\nHeader header\nstring child_frame_id\ngeometry_msgs/PoseWithCovariance pose\ngeometry_msgs/TwistWithCovariance twist\n")
    .def_readwrite("header", &Odometry::header)
    .def_readwrite("child_frame_id", &Odometry::child_frame_id)
    .def_readwrite("pose", &Odometry::pose)
    .def_readwrite("twist", &Odometry::twist)
    ;

}

} // namespace

#include <geometry_msgs/Pose2D.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Pose2D& m1, const Pose2D& m2)
{
  return false;
}

void exportPose2D ()
{
  using geometry_msgs::Pose2D;
  class_<Pose2D, shared_ptr<Pose2D> >("Pose2D", "# Deprecated\n# Please use the full 3D pose.\n\n# In general our recommendation is to use a full 3D representation of everything and for 2D specific applications make the appropriate projections into the plane for their calculations but optimally will preserve the 3D information during processing.\n\n# If we have parallel copies of 2D datatypes every UI and other pipeline will end up needing to have dual interfaces to plot everything. And you will end up with not being able to use 3D tools for 2D use cases even if they're completely valid, as you'd have to reimplement it with different inputs and outputs. It's not particularly hard to plot the 2D pose or compute the yaw error for the Pose message and there are already tools and libraries that can do this for you.\n\n\n# This expresses a position and orientation on a 2D manifold.\n\nfloat64 x\nfloat64 y\nfloat64 theta\n")
    .def_readwrite("x", &Pose2D::x)
    .def_readwrite("y", &Pose2D::y)
    .def_readwrite("theta", &Pose2D::theta)
    ;

}

} // namespace
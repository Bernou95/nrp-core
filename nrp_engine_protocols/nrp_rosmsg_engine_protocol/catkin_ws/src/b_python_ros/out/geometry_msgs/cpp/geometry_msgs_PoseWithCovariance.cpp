
#include <geometry_msgs/PoseWithCovariance.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const PoseWithCovariance& m1, const PoseWithCovariance& m2)
{
  return false;
}

void exportPoseWithCovariance ()
{
  using geometry_msgs::PoseWithCovariance;
  class_<PoseWithCovariance, shared_ptr<PoseWithCovariance> >("PoseWithCovariance", "# This represents a pose in free space with uncertainty.\n\nPose pose\n\n# Row-major representation of the 6x6 covariance matrix\n# The orientation parameters use a fixed-axis representation.\n# In order, the parameters are:\n# (x, y, z, rotation about X axis, rotation about Y axis, rotation about Z axis)\nfloat64[36] covariance\n")
    .def_readwrite("pose", &PoseWithCovariance::pose)
    .def_readwrite("covariance", &PoseWithCovariance::covariance)
    ;

  class_<vector<double> >("PoseWithCovariance_covariance")
    .def(vector_indexing_suite<vector<double> > ())
    ;
}

} // namespace
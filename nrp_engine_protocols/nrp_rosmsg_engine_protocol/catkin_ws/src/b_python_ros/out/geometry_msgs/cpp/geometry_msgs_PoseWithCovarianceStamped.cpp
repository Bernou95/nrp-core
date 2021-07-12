
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const PoseWithCovarianceStamped& m1, const PoseWithCovarianceStamped& m2)
{
  return false;
}

void exportPoseWithCovarianceStamped ()
{
  using geometry_msgs::PoseWithCovarianceStamped;
  class_<PoseWithCovarianceStamped, shared_ptr<PoseWithCovarianceStamped> >("PoseWithCovarianceStamped", "# This expresses an estimated pose with a reference coordinate frame and timestamp\n\nHeader header\nPoseWithCovariance pose\n")
    .def_readwrite("header", &PoseWithCovarianceStamped::header)
    .def_readwrite("pose", &PoseWithCovarianceStamped::pose)
    ;

}

} // namespace

#include <geometry_msgs/PoseArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs

{
inline bool operator== (const geometry_msgs::Pose& m1, const geometry_msgs::Pose& m2)
{
  return false;
}

} // namespace
namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const PoseArray& m1, const PoseArray& m2)
{
  return false;
}

void exportPoseArray ()
{
  using geometry_msgs::PoseArray;
  class_<PoseArray, shared_ptr<PoseArray> >("PoseArray", "# An array of poses with a header for global reference.\n\nHeader header\n\nPose[] poses\n")
    .def_readwrite("header", &PoseArray::header)
    .def_readwrite("poses", &PoseArray::poses)
    ;

  class_<vector<geometry_msgs::Pose> >("PoseArray_poses")
    .def(vector_indexing_suite<vector<geometry_msgs::Pose> > ())
    ;
}

} // namespace
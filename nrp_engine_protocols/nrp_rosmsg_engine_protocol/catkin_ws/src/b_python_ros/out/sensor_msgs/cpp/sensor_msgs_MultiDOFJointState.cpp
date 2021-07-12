
#include <sensor_msgs/MultiDOFJointState.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs

{
inline bool operator== (const geometry_msgs::Transform& m1, const geometry_msgs::Transform& m2)
{
  return false;
}

} // namespace
namespace geometry_msgs

{
inline bool operator== (const geometry_msgs::Twist& m1, const geometry_msgs::Twist& m2)
{
  return false;
}

} // namespace
namespace geometry_msgs

{
inline bool operator== (const geometry_msgs::Wrench& m1, const geometry_msgs::Wrench& m2)
{
  return false;
}

} // namespace
namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const MultiDOFJointState& m1, const MultiDOFJointState& m2)
{
  return false;
}

void exportMultiDOFJointState ()
{
  using sensor_msgs::MultiDOFJointState;
  class_<MultiDOFJointState, shared_ptr<MultiDOFJointState> >("MultiDOFJointState", "# Representation of state for joints with multiple degrees of freedom, \n# following the structure of JointState.\n#\n# It is assumed that a joint in a system corresponds to a transform that gets applied \n# along the kinematic chain. For example, a planar joint (as in URDF) is 3DOF (x, y, yaw)\n# and those 3DOF can be expressed as a transformation matrix, and that transformation\n# matrix can be converted back to (x, y, yaw)\n#\n# Each joint is uniquely identified by its name\n# The header specifies the time at which the joint states were recorded. All the joint states\n# in one message have to be recorded at the same time.\n#\n# This message consists of a multiple arrays, one for each part of the joint state. \n# The goal is to make each of the fields optional. When e.g. your joints have no\n# wrench associated with them, you can leave the wrench array empty. \n#\n# All arrays in this message should have the same size, or be empty.\n# This is the only way to uniquely associate the joint name with the correct\n# states.\n\nHeader header\n\nstring[] joint_names\ngeometry_msgs/Transform[] transforms\ngeometry_msgs/Twist[] twist\ngeometry_msgs/Wrench[] wrench\n")
    .def_readwrite("header", &MultiDOFJointState::header)
    .def_readwrite("joint_names", &MultiDOFJointState::joint_names)
    .def_readwrite("transforms", &MultiDOFJointState::transforms)
    .def_readwrite("twist", &MultiDOFJointState::twist)
    .def_readwrite("wrench", &MultiDOFJointState::wrench)
    ;

  class_<vector<std::string> >("MultiDOFJointState_joint_names")
    .def(vector_indexing_suite<vector<std::string> > ())
    ;
  class_<vector<geometry_msgs::Transform> >("MultiDOFJointState_transforms")
    .def(vector_indexing_suite<vector<geometry_msgs::Transform> > ())
    ;
  class_<vector<geometry_msgs::Twist> >("MultiDOFJointState_twist")
    .def(vector_indexing_suite<vector<geometry_msgs::Twist> > ())
    ;
  class_<vector<geometry_msgs::Wrench> >("MultiDOFJointState_wrench")
    .def(vector_indexing_suite<vector<geometry_msgs::Wrench> > ())
    ;
}

} // namespace
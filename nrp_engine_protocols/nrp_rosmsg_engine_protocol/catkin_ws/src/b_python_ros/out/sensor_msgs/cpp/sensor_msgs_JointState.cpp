
#include <sensor_msgs/JointState.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const JointState& m1, const JointState& m2)
{
  return false;
}

void exportJointState ()
{
  using sensor_msgs::JointState;
  class_<JointState, shared_ptr<JointState> >("JointState", "# This is a message that holds data to describe the state of a set of torque controlled joints. \n#\n# The state of each joint (revolute or prismatic) is defined by:\n#  * the position of the joint (rad or m),\n#  * the velocity of the joint (rad/s or m/s) and \n#  * the effort that is applied in the joint (Nm or N).\n#\n# Each joint is uniquely identified by its name\n# The header specifies the time at which the joint states were recorded. All the joint states\n# in one message have to be recorded at the same time.\n#\n# This message consists of a multiple arrays, one for each part of the joint state. \n# The goal is to make each of the fields optional. When e.g. your joints have no\n# effort associated with them, you can leave the effort array empty. \n#\n# All arrays in this message should have the same size, or be empty.\n# This is the only way to uniquely associate the joint name with the correct\n# states.\n\n\nHeader header\n\nstring[] name\nfloat64[] position\nfloat64[] velocity\nfloat64[] effort\n")
    .def_readwrite("header", &JointState::header)
    .def_readwrite("name", &JointState::name)
    .def_readwrite("position", &JointState::position)
    .def_readwrite("velocity", &JointState::velocity)
    .def_readwrite("effort", &JointState::effort)
    ;

  class_<vector<std::string> >("JointState_name")
    .def(vector_indexing_suite<vector<std::string> > ())
    ;
  class_<vector<double> >("JointState_position")
    .def(vector_indexing_suite<vector<double> > ())
    ;
  class_<vector<double> >("JointState_velocity")
    .def(vector_indexing_suite<vector<double> > ())
    ;
  class_<vector<double> >("JointState_effort")
    .def(vector_indexing_suite<vector<double> > ())
    ;
}

} // namespace
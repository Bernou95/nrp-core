
#include <sensor_msgs/JoyFeedbackArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs

{
inline bool operator== (const sensor_msgs::JoyFeedback& m1, const sensor_msgs::JoyFeedback& m2)
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
bool operator== (const JoyFeedbackArray& m1, const JoyFeedbackArray& m2)
{
  return false;
}

void exportJoyFeedbackArray ()
{
  using sensor_msgs::JoyFeedbackArray;
  class_<JoyFeedbackArray, shared_ptr<JoyFeedbackArray> >("JoyFeedbackArray", "# This message publishes values for multiple feedback at once. \nJoyFeedback[] array")
    .def_readwrite("array", &JoyFeedbackArray::array)
    ;

  class_<vector<sensor_msgs::JoyFeedback> >("JoyFeedbackArray_array")
    .def(vector_indexing_suite<vector<sensor_msgs::JoyFeedback> > ())
    ;
}

} // namespace
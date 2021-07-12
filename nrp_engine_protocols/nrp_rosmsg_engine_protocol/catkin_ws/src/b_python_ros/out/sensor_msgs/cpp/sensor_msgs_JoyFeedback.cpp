
#include <sensor_msgs/JoyFeedback.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const JoyFeedback& m1, const JoyFeedback& m2)
{
  return false;
}

void exportJoyFeedback ()
{
  using sensor_msgs::JoyFeedback;
  class_<JoyFeedback, shared_ptr<JoyFeedback> >("JoyFeedback", "# Declare of the type of feedback\nuint8 TYPE_LED    = 0\nuint8 TYPE_RUMBLE = 1\nuint8 TYPE_BUZZER = 2\n\nuint8 type\n\n# This will hold an id number for each type of each feedback.\n# Example, the first led would be id=0, the second would be id=1\nuint8 id\n\n# Intensity of the feedback, from 0.0 to 1.0, inclusive.  If device is\n# actually binary, driver should treat 0<=x<0.5 as off, 0.5<=x<=1 as on.\nfloat32 intensity\n\n")
    .def_readwrite("type", &JoyFeedback::type)
    .def_readwrite("id", &JoyFeedback::id)
    .def_readwrite("intensity", &JoyFeedback::intensity)
    ;

}

} // namespace
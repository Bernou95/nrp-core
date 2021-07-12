
#include <sensor_msgs/Joy.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Joy& m1, const Joy& m2)
{
  return false;
}

void exportJoy ()
{
  using sensor_msgs::Joy;
  class_<Joy, shared_ptr<Joy> >("Joy", "# Reports the state of a joysticks axes and buttons.\nHeader header           # timestamp in the header is the time the data is received from the joystick\nfloat32[] axes          # the axes measurements from a joystick\nint32[] buttons         # the buttons measurements from a joystick \n")
    .def_readwrite("header", &Joy::header)
    .def_readwrite("axes", &Joy::axes)
    .def_readwrite("buttons", &Joy::buttons)
    ;

  class_<vector<float> >("Joy_axes")
    .def(vector_indexing_suite<vector<float> > ())
    ;
  class_<vector<int32_t> >("Joy_buttons")
    .def(vector_indexing_suite<vector<int32_t> > ())
    ;
}

} // namespace
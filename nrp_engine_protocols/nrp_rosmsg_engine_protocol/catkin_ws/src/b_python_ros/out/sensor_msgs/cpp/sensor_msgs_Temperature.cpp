
#include <sensor_msgs/Temperature.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Temperature& m1, const Temperature& m2)
{
  return false;
}

void exportTemperature ()
{
  using sensor_msgs::Temperature;
  class_<Temperature, shared_ptr<Temperature> >("Temperature", " # Single temperature reading.\n\n Header header           # timestamp is the time the temperature was measured\n                         # frame_id is the location of the temperature reading\n\n float64 temperature     # Measurement of the Temperature in Degrees Celsius\n\n float64 variance        # 0 is interpreted as variance unknown")
    .def_readwrite("header", &Temperature::header)
    .def_readwrite("temperature", &Temperature::temperature)
    .def_readwrite("variance", &Temperature::variance)
    ;

}

} // namespace
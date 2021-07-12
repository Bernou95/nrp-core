
#include <sensor_msgs/RelativeHumidity.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const RelativeHumidity& m1, const RelativeHumidity& m2)
{
  return false;
}

void exportRelativeHumidity ()
{
  using sensor_msgs::RelativeHumidity;
  class_<RelativeHumidity, shared_ptr<RelativeHumidity> >("RelativeHumidity", " # Single reading from a relative humidity sensor.  Defines the ratio of partial\n # pressure of water vapor to the saturated vapor pressure at a temperature.\n\n Header header             # timestamp of the measurement\n                           # frame_id is the location of the humidity sensor\n\n float64 relative_humidity # Expression of the relative humidity\n                           # from 0.0 to 1.0.\n                           # 0.0 is no partial pressure of water vapor\n                           # 1.0 represents partial pressure of saturation\n\n float64 variance          # 0 is interpreted as variance unknown")
    .def_readwrite("header", &RelativeHumidity::header)
    .def_readwrite("relative_humidity", &RelativeHumidity::relative_humidity)
    .def_readwrite("variance", &RelativeHumidity::variance)
    ;

}

} // namespace

#include <sensor_msgs/FluidPressure.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const FluidPressure& m1, const FluidPressure& m2)
{
  return false;
}

void exportFluidPressure ()
{
  using sensor_msgs::FluidPressure;
  class_<FluidPressure, shared_ptr<FluidPressure> >("FluidPressure", " # Single pressure reading.  This message is appropriate for measuring the\n # pressure inside of a fluid (air, water, etc).  This also includes\n # atmospheric or barometric pressure.\n\n # This message is not appropriate for force/pressure contact sensors.\n\n Header header           # timestamp of the measurement\n                         # frame_id is the location of the pressure sensor\n\n float64 fluid_pressure  # Absolute pressure reading in Pascals.\n\n float64 variance        # 0 is interpreted as variance unknown")
    .def_readwrite("header", &FluidPressure::header)
    .def_readwrite("fluid_pressure", &FluidPressure::fluid_pressure)
    .def_readwrite("variance", &FluidPressure::variance)
    ;

}

} // namespace
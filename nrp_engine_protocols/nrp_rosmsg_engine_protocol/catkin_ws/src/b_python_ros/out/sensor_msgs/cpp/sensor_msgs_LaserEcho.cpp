
#include <sensor_msgs/LaserEcho.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const LaserEcho& m1, const LaserEcho& m2)
{
  return false;
}

void exportLaserEcho ()
{
  using sensor_msgs::LaserEcho;
  class_<LaserEcho, shared_ptr<LaserEcho> >("LaserEcho", "# This message is a submessage of MultiEchoLaserScan and is not intended\n# to be used separately.\n\nfloat32[] echoes  # Multiple values of ranges or intensities.\n                  # Each array represents data from the same angle increment.")
    .def_readwrite("echoes", &LaserEcho::echoes)
    ;

  class_<vector<float> >("LaserEcho_echoes")
    .def(vector_indexing_suite<vector<float> > ())
    ;
}

} // namespace
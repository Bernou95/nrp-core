
#include <sensor_msgs/MultiEchoLaserScan.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs

{
inline bool operator== (const sensor_msgs::LaserEcho& m1, const sensor_msgs::LaserEcho& m2)
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
bool operator== (const MultiEchoLaserScan& m1, const MultiEchoLaserScan& m2)
{
  return false;
}

void exportMultiEchoLaserScan ()
{
  using sensor_msgs::MultiEchoLaserScan;
  class_<MultiEchoLaserScan, shared_ptr<MultiEchoLaserScan> >("MultiEchoLaserScan", "# Single scan from a multi-echo planar laser range-finder\n#\n# If you have another ranging device with different behavior (e.g. a sonar\n# array), please find or create a different message, since applications\n# will make fairly laser-specific assumptions about this data\n\nHeader header            # timestamp in the header is the acquisition time of \n                         # the first ray in the scan.\n                         #\n                         # in frame frame_id, angles are measured around \n                         # the positive Z axis (counterclockwise, if Z is up)\n                         # with zero angle being forward along the x axis\n                         \nfloat32 angle_min        # start angle of the scan [rad]\nfloat32 angle_max        # end angle of the scan [rad]\nfloat32 angle_increment  # angular distance between measurements [rad]\n\nfloat32 time_increment   # time between measurements [seconds] - if your scanner\n                         # is moving, this will be used in interpolating position\n                         # of 3d points\nfloat32 scan_time        # time between scans [seconds]\n\nfloat32 range_min        # minimum range value [m]\nfloat32 range_max        # maximum range value [m]\n\nLaserEcho[] ranges       # range data [m] (Note: NaNs, values < range_min or > range_max should be discarded)\n                         # +Inf measurements are out of range\n                         # -Inf measurements are too close to determine exact distance.\nLaserEcho[] intensities  # intensity data [device-specific units].  If your\n                         # device does not provide intensities, please leave\n                         # the array empty.")
    .def_readwrite("header", &MultiEchoLaserScan::header)
    .def_readwrite("angle_min", &MultiEchoLaserScan::angle_min)
    .def_readwrite("angle_max", &MultiEchoLaserScan::angle_max)
    .def_readwrite("angle_increment", &MultiEchoLaserScan::angle_increment)
    .def_readwrite("time_increment", &MultiEchoLaserScan::time_increment)
    .def_readwrite("scan_time", &MultiEchoLaserScan::scan_time)
    .def_readwrite("range_min", &MultiEchoLaserScan::range_min)
    .def_readwrite("range_max", &MultiEchoLaserScan::range_max)
    .def_readwrite("ranges", &MultiEchoLaserScan::ranges)
    .def_readwrite("intensities", &MultiEchoLaserScan::intensities)
    ;

  class_<vector<sensor_msgs::LaserEcho> >("MultiEchoLaserScan_ranges")
    .def(vector_indexing_suite<vector<sensor_msgs::LaserEcho> > ())
    ;
  class_<vector<sensor_msgs::LaserEcho> >("MultiEchoLaserScan_intensities")
    .def(vector_indexing_suite<vector<sensor_msgs::LaserEcho> > ())
    ;
}

} // namespace
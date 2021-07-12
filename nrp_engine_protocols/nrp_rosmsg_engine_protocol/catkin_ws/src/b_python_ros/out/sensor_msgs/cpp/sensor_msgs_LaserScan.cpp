
#include <sensor_msgs/LaserScan.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const LaserScan& m1, const LaserScan& m2)
{
  return false;
}

void exportLaserScan ()
{
  using sensor_msgs::LaserScan;
  class_<LaserScan, shared_ptr<LaserScan> >("LaserScan", "# Single scan from a planar laser range-finder\n#\n# If you have another ranging device with different behavior (e.g. a sonar\n# array), please find or create a different message, since applications\n# will make fairly laser-specific assumptions about this data\n\nHeader header            # timestamp in the header is the acquisition time of \n                         # the first ray in the scan.\n                         #\n                         # in frame frame_id, angles are measured around \n                         # the positive Z axis (counterclockwise, if Z is up)\n                         # with zero angle being forward along the x axis\n                         \nfloat32 angle_min        # start angle of the scan [rad]\nfloat32 angle_max        # end angle of the scan [rad]\nfloat32 angle_increment  # angular distance between measurements [rad]\n\nfloat32 time_increment   # time between measurements [seconds] - if your scanner\n                         # is moving, this will be used in interpolating position\n                         # of 3d points\nfloat32 scan_time        # time between scans [seconds]\n\nfloat32 range_min        # minimum range value [m]\nfloat32 range_max        # maximum range value [m]\n\nfloat32[] ranges         # range data [m] (Note: values < range_min or > range_max should be discarded)\nfloat32[] intensities    # intensity data [device-specific units].  If your\n                         # device does not provide intensities, please leave\n                         # the array empty.\n")
    .def_readwrite("header", &LaserScan::header)
    .def_readwrite("angle_min", &LaserScan::angle_min)
    .def_readwrite("angle_max", &LaserScan::angle_max)
    .def_readwrite("angle_increment", &LaserScan::angle_increment)
    .def_readwrite("time_increment", &LaserScan::time_increment)
    .def_readwrite("scan_time", &LaserScan::scan_time)
    .def_readwrite("range_min", &LaserScan::range_min)
    .def_readwrite("range_max", &LaserScan::range_max)
    .def_readwrite("ranges", &LaserScan::ranges)
    .def_readwrite("intensities", &LaserScan::intensities)
    ;

  class_<vector<float> >("LaserScan_ranges")
    .def(vector_indexing_suite<vector<float> > ())
    ;
  class_<vector<float> >("LaserScan_intensities")
    .def(vector_indexing_suite<vector<float> > ())
    ;
}

} // namespace
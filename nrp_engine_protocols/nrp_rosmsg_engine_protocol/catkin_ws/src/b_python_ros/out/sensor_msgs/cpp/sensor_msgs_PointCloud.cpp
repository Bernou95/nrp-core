
#include <sensor_msgs/PointCloud.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs

{
inline bool operator== (const geometry_msgs::Point32& m1, const geometry_msgs::Point32& m2)
{
  return false;
}

} // namespace
namespace sensor_msgs

{
inline bool operator== (const sensor_msgs::ChannelFloat32& m1, const sensor_msgs::ChannelFloat32& m2)
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
bool operator== (const PointCloud& m1, const PointCloud& m2)
{
  return false;
}

void exportPointCloud ()
{
  using sensor_msgs::PointCloud;
  class_<PointCloud, shared_ptr<PointCloud> >("PointCloud", "# This message holds a collection of 3d points, plus optional additional\n# information about each point.\n\n# Time of sensor data acquisition, coordinate frame ID.\nHeader header\n\n# Array of 3d points. Each Point32 should be interpreted as a 3d point\n# in the frame given in the header.\ngeometry_msgs/Point32[] points\n\n# Each channel should have the same number of elements as points array,\n# and the data in each channel should correspond 1:1 with each point.\n# Channel names in common practice are listed in ChannelFloat32.msg.\nChannelFloat32[] channels\n")
    .def_readwrite("header", &PointCloud::header)
    .def_readwrite("points", &PointCloud::points)
    .def_readwrite("channels", &PointCloud::channels)
    ;

  class_<vector<geometry_msgs::Point32> >("PointCloud_points")
    .def(vector_indexing_suite<vector<geometry_msgs::Point32> > ())
    ;
  class_<vector<sensor_msgs::ChannelFloat32> >("PointCloud_channels")
    .def(vector_indexing_suite<vector<sensor_msgs::ChannelFloat32> > ())
    ;
}

} // namespace
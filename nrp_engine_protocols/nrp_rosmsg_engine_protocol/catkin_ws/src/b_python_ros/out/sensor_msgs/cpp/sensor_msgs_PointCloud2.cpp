
#include <sensor_msgs/PointCloud2.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs

{
inline bool operator== (const sensor_msgs::PointField& m1, const sensor_msgs::PointField& m2)
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
bool operator== (const PointCloud2& m1, const PointCloud2& m2)
{
  return false;
}

void exportPointCloud2 ()
{
  using sensor_msgs::PointCloud2;
  class_<PointCloud2, shared_ptr<PointCloud2> >("PointCloud2", "# This message holds a collection of N-dimensional points, which may\n# contain additional information such as normals, intensity, etc. The\n# point data is stored as a binary blob, its layout described by the\n# contents of the \"fields\" array.\n\n# The point cloud data may be organized 2d (image-like) or 1d\n# (unordered). Point clouds organized as 2d images may be produced by\n# camera depth sensors such as stereo or time-of-flight.\n\n# Time of sensor data acquisition, and the coordinate frame ID (for 3d\n# points).\nHeader header\n\n# 2D structure of the point cloud. If the cloud is unordered, height is\n# 1 and width is the length of the point cloud.\nuint32 height\nuint32 width\n\n# Describes the channels and their layout in the binary data blob.\nPointField[] fields\n\nbool    is_bigendian # Is this data bigendian?\nuint32  point_step   # Length of a point in bytes\nuint32  row_step     # Length of a row in bytes\nuint8[] data         # Actual point data, size is (row_step*height)\n\nbool is_dense        # True if there are no invalid points\n")
    .def_readwrite("header", &PointCloud2::header)
    .def_readwrite("height", &PointCloud2::height)
    .def_readwrite("width", &PointCloud2::width)
    .def_readwrite("fields", &PointCloud2::fields)
    .def_readwrite("is_bigendian", &PointCloud2::is_bigendian)
    .def_readwrite("point_step", &PointCloud2::point_step)
    .def_readwrite("row_step", &PointCloud2::row_step)
    .def_readwrite("data", &PointCloud2::data)
    .def_readwrite("is_dense", &PointCloud2::is_dense)
    ;

  class_<vector<sensor_msgs::PointField> >("PointCloud2_fields")
    .def(vector_indexing_suite<vector<sensor_msgs::PointField> > ())
    ;
  class_<vector<uint8_t> >("PointCloud2_data")
    .def(vector_indexing_suite<vector<uint8_t> > ())
    ;
}

} // namespace
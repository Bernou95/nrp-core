
#include <sensor_msgs/CompressedImage.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const CompressedImage& m1, const CompressedImage& m2)
{
  return false;
}

void exportCompressedImage ()
{
  using sensor_msgs::CompressedImage;
  class_<CompressedImage, shared_ptr<CompressedImage> >("CompressedImage", "# This message contains a compressed image\n\nHeader header        # Header timestamp should be acquisition time of image\n                     # Header frame_id should be optical frame of camera\n                     # origin of frame should be optical center of camera\n                     # +x should point to the right in the image\n                     # +y should point down in the image\n                     # +z should point into to plane of the image\n\nstring format        # Specifies the format of the data\n                     #   Acceptable values:\n                     #     jpeg, png\nuint8[] data         # Compressed image buffer\n")
    .def_readwrite("header", &CompressedImage::header)
    .def_readwrite("format", &CompressedImage::format)
    .def_readwrite("data", &CompressedImage::data)
    ;

  class_<vector<uint8_t> >("CompressedImage_data")
    .def(vector_indexing_suite<vector<uint8_t> > ())
    ;
}

} // namespace

#include <sensor_msgs/Image.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Image& m1, const Image& m2)
{
  return false;
}

void exportImage ()
{
  using sensor_msgs::Image;
  class_<Image, shared_ptr<Image> >("Image", "# This message contains an uncompressed image\n# (0, 0) is at top-left corner of image\n#\n\nHeader header        # Header timestamp should be acquisition time of image\n                     # Header frame_id should be optical frame of camera\n                     # origin of frame should be optical center of camera\n                     # +x should point to the right in the image\n                     # +y should point down in the image\n                     # +z should point into to plane of the image\n                     # If the frame_id here and the frame_id of the CameraInfo\n                     # message associated with the image conflict\n                     # the behavior is undefined\n\nuint32 height         # image height, that is, number of rows\nuint32 width          # image width, that is, number of columns\n\n# The legal values for encoding are in file src/image_encodings.cpp\n# If you want to standardize a new string format, join\n# ros-users@lists.sourceforge.net and send an email proposing a new encoding.\n\nstring encoding       # Encoding of pixels -- channel meaning, ordering, size\n                      # taken from the list of strings in include/sensor_msgs/image_encodings.h\n\nuint8 is_bigendian    # is this data bigendian?\nuint32 step           # Full row length in bytes\nuint8[] data          # actual matrix data, size is (step * rows)\n")
    .def_readwrite("header", &Image::header)
    .def_readwrite("height", &Image::height)
    .def_readwrite("width", &Image::width)
    .def_readwrite("encoding", &Image::encoding)
    .def_readwrite("is_bigendian", &Image::is_bigendian)
    .def_readwrite("step", &Image::step)
    .def_readwrite("data", &Image::data)
    ;

  class_<vector<uint8_t> >("Image_data")
    .def(vector_indexing_suite<vector<uint8_t> > ())
    ;
}

} // namespace
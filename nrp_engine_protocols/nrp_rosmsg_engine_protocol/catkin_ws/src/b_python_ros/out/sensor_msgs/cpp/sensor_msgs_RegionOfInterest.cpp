
#include <sensor_msgs/RegionOfInterest.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const RegionOfInterest& m1, const RegionOfInterest& m2)
{
  return false;
}

void exportRegionOfInterest ()
{
  using sensor_msgs::RegionOfInterest;
  class_<RegionOfInterest, shared_ptr<RegionOfInterest> >("RegionOfInterest", "# This message is used to specify a region of interest within an image.\n#\n# When used to specify the ROI setting of the camera when the image was\n# taken, the height and width fields should either match the height and\n# width fields for the associated image; or height = width = 0\n# indicates that the full resolution image was captured.\n\nuint32 x_offset  # Leftmost pixel of the ROI\n                 # (0 if the ROI includes the left edge of the image)\nuint32 y_offset  # Topmost pixel of the ROI\n                 # (0 if the ROI includes the top edge of the image)\nuint32 height    # Height of ROI\nuint32 width     # Width of ROI\n\n# True if a distinct rectified ROI should be calculated from the \"raw\"\n# ROI in this message. Typically this should be False if the full image\n# is captured (ROI not used), and True if a subwindow is captured (ROI\n# used).\nbool do_rectify\n")
    .def_readwrite("x_offset", &RegionOfInterest::x_offset)
    .def_readwrite("y_offset", &RegionOfInterest::y_offset)
    .def_readwrite("height", &RegionOfInterest::height)
    .def_readwrite("width", &RegionOfInterest::width)
    .def_readwrite("do_rectify", &RegionOfInterest::do_rectify)
    ;

}

} // namespace
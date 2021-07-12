
#include <sensor_msgs/ChannelFloat32.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const ChannelFloat32& m1, const ChannelFloat32& m2)
{
  return false;
}

void exportChannelFloat32 ()
{
  using sensor_msgs::ChannelFloat32;
  class_<ChannelFloat32, shared_ptr<ChannelFloat32> >("ChannelFloat32", "# This message is used by the PointCloud message to hold optional data\n# associated with each point in the cloud. The length of the values\n# array should be the same as the length of the points array in the\n# PointCloud, and each value should be associated with the corresponding\n# point.\n\n# Channel names in existing practice include:\n#   \"u\", \"v\" - row and column (respectively) in the left stereo image.\n#              This is opposite to usual conventions but remains for\n#              historical reasons. The newer PointCloud2 message has no\n#              such problem.\n#   \"rgb\" - For point clouds produced by color stereo cameras. uint8\n#           (R,G,B) values packed into the least significant 24 bits,\n#           in order.\n#   \"intensity\" - laser or pixel intensity.\n#   \"distance\"\n\n# The channel name should give semantics of the channel (e.g.\n# \"intensity\" instead of \"value\").\nstring name\n\n# The values array should be 1-1 with the elements of the associated\n# PointCloud.\nfloat32[] values\n")
    .def_readwrite("name", &ChannelFloat32::name)
    .def_readwrite("values", &ChannelFloat32::values)
    ;

  class_<vector<float> >("ChannelFloat32_values")
    .def(vector_indexing_suite<vector<float> > ())
    ;
}

} // namespace
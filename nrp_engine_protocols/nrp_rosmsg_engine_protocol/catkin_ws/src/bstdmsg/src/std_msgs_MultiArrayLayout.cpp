
#include <std_msgs/MultiArrayLayout.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs

{
inline bool operator== (const std_msgs::MultiArrayDimension& m1, const std_msgs::MultiArrayDimension& m2)
{
  return false;
}

} // namespace
namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const MultiArrayLayout& m1, const MultiArrayLayout& m2)
{
  return false;
}

void exportMultiArrayLayout ()
{
  using std_msgs::MultiArrayLayout;
  class_<MultiArrayLayout, shared_ptr<MultiArrayLayout> >("MultiArrayLayout", "# The multiarray declares a generic multi-dimensional array of a\n# particular data type.  Dimensions are ordered from outer most\n# to inner most.\n\nMultiArrayDimension[] dim # Array of dimension properties\nuint32 data_offset        # padding elements at front of data\n\n# Accessors should ALWAYS be written in terms of dimension stride\n# and specified outer-most dimension first.\n# \n# multiarray(i,j,k) = data[data_offset + dim_stride[1]*i + dim_stride[2]*j + k]\n#\n# A standard, 3-channel 640x480 image with interleaved color channels\n# would be specified as:\n#\n# dim[0].label  = \"height\"\n# dim[0].size   = 480\n# dim[0].stride = 3*640*480 = 921600  (note dim[0] stride is just size of image)\n# dim[1].label  = \"width\"\n# dim[1].size   = 640\n# dim[1].stride = 3*640 = 1920\n# dim[2].label  = \"channel\"\n# dim[2].size   = 3\n# dim[2].stride = 3\n#\n# multiarray(i,j,k) refers to the ith row, jth column, and kth channel.\n")
    .def_readwrite("dim", &MultiArrayLayout::dim)
    .def_readwrite("data_offset", &MultiArrayLayout::data_offset)
    ;

  class_<vector<std_msgs::MultiArrayDimension> >("MultiArrayLayout_dim")
    .def(vector_indexing_suite<vector<std_msgs::MultiArrayDimension> > ())
    ;
}

} // namespace
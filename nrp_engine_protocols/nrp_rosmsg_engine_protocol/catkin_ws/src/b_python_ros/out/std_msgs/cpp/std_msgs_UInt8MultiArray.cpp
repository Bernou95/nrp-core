
#include <std_msgs/UInt8MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const UInt8MultiArray& m1, const UInt8MultiArray& m2)
{
  return false;
}

void exportUInt8MultiArray ()
{
  using std_msgs::UInt8MultiArray;
  class_<UInt8MultiArray, shared_ptr<UInt8MultiArray> >("UInt8MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nuint8[]           data          # array of data\n\n")
    .def_readwrite("layout", &UInt8MultiArray::layout)
    .def_readwrite("data", &UInt8MultiArray::data)
    ;

  class_<vector<uint8_t> >("UInt8MultiArray_data")
    .def(vector_indexing_suite<vector<uint8_t> > ())
    ;
}

} // namespace
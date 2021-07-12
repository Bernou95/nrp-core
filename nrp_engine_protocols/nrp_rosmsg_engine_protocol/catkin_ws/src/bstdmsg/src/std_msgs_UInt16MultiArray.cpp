
#include <std_msgs/UInt16MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const UInt16MultiArray& m1, const UInt16MultiArray& m2)
{
  return false;
}

void exportUInt16MultiArray ()
{
  using std_msgs::UInt16MultiArray;
  class_<UInt16MultiArray, shared_ptr<UInt16MultiArray> >("UInt16MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nuint16[]            data        # array of data\n\n")
    .def_readwrite("layout", &UInt16MultiArray::layout)
    .def_readwrite("data", &UInt16MultiArray::data)
    ;

  class_<vector<uint16_t> >("UInt16MultiArray_data")
    .def(vector_indexing_suite<vector<uint16_t> > ())
    ;
}

} // namespace
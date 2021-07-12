
#include <std_msgs/ByteMultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const ByteMultiArray& m1, const ByteMultiArray& m2)
{
  return false;
}

void exportByteMultiArray ()
{
  using std_msgs::ByteMultiArray;
  class_<ByteMultiArray, shared_ptr<ByteMultiArray> >("ByteMultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nbyte[]            data          # array of data\n\n")
    .def_readwrite("layout", &ByteMultiArray::layout)
    .def_readwrite("data", &ByteMultiArray::data)
    ;

  class_<vector<int8_t> >("ByteMultiArray_data")
    .def(vector_indexing_suite<vector<int8_t> > ())
    ;
}

} // namespace
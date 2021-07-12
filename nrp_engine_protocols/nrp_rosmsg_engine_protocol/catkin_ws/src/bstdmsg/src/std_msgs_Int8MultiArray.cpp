
#include <std_msgs/Int8MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Int8MultiArray& m1, const Int8MultiArray& m2)
{
  return false;
}

void exportInt8MultiArray ()
{
  using std_msgs::Int8MultiArray;
  class_<Int8MultiArray, shared_ptr<Int8MultiArray> >("Int8MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nint8[]            data          # array of data\n\n")
    .def_readwrite("layout", &Int8MultiArray::layout)
    .def_readwrite("data", &Int8MultiArray::data)
    ;

  class_<vector<int8_t> >("Int8MultiArray_data")
    .def(vector_indexing_suite<vector<int8_t> > ())
    ;
}

} // namespace
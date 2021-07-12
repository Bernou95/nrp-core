
#include <std_msgs/UInt64MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const UInt64MultiArray& m1, const UInt64MultiArray& m2)
{
  return false;
}

void exportUInt64MultiArray ()
{
  using std_msgs::UInt64MultiArray;
  class_<UInt64MultiArray, shared_ptr<UInt64MultiArray> >("UInt64MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nuint64[]          data          # array of data\n\n")
    .def_readwrite("layout", &UInt64MultiArray::layout)
    .def_readwrite("data", &UInt64MultiArray::data)
    ;

  class_<vector<uint64_t> >("UInt64MultiArray_data")
    .def(vector_indexing_suite<vector<uint64_t> > ())
    ;
}

} // namespace
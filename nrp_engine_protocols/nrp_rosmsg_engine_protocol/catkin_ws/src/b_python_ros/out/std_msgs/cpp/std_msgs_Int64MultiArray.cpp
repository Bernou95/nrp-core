
#include <std_msgs/Int64MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Int64MultiArray& m1, const Int64MultiArray& m2)
{
  return false;
}

void exportInt64MultiArray ()
{
  using std_msgs::Int64MultiArray;
  class_<Int64MultiArray, shared_ptr<Int64MultiArray> >("Int64MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nint64[]           data          # array of data\n\n")
    .def_readwrite("layout", &Int64MultiArray::layout)
    .def_readwrite("data", &Int64MultiArray::data)
    ;

  class_<vector<int64_t> >("Int64MultiArray_data")
    .def(vector_indexing_suite<vector<int64_t> > ())
    ;
}

} // namespace
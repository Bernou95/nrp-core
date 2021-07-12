
#include <std_msgs/Int16MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Int16MultiArray& m1, const Int16MultiArray& m2)
{
  return false;
}

void exportInt16MultiArray ()
{
  using std_msgs::Int16MultiArray;
  class_<Int16MultiArray, shared_ptr<Int16MultiArray> >("Int16MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nint16[]           data          # array of data\n\n")
    .def_readwrite("layout", &Int16MultiArray::layout)
    .def_readwrite("data", &Int16MultiArray::data)
    ;

  class_<vector<int16_t> >("Int16MultiArray_data")
    .def(vector_indexing_suite<vector<int16_t> > ())
    ;
}

} // namespace
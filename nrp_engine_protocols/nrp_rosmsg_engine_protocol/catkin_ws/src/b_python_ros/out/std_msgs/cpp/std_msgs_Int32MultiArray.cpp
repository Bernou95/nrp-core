
#include <std_msgs/Int32MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Int32MultiArray& m1, const Int32MultiArray& m2)
{
  return false;
}

void exportInt32MultiArray ()
{
  using std_msgs::Int32MultiArray;
  class_<Int32MultiArray, shared_ptr<Int32MultiArray> >("Int32MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nint32[]           data          # array of data\n\n")
    .def_readwrite("layout", &Int32MultiArray::layout)
    .def_readwrite("data", &Int32MultiArray::data)
    ;

  class_<vector<int32_t> >("Int32MultiArray_data")
    .def(vector_indexing_suite<vector<int32_t> > ())
    ;
}

} // namespace
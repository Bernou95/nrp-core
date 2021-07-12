
#include <std_msgs/UInt32MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const UInt32MultiArray& m1, const UInt32MultiArray& m2)
{
  return false;
}

void exportUInt32MultiArray ()
{
  using std_msgs::UInt32MultiArray;
  class_<UInt32MultiArray, shared_ptr<UInt32MultiArray> >("UInt32MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nuint32[]          data          # array of data\n\n")
    .def_readwrite("layout", &UInt32MultiArray::layout)
    .def_readwrite("data", &UInt32MultiArray::data)
    ;

  class_<vector<uint32_t> >("UInt32MultiArray_data")
    .def(vector_indexing_suite<vector<uint32_t> > ())
    ;
}

} // namespace
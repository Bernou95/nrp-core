
#include <std_msgs/Float32MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Float32MultiArray& m1, const Float32MultiArray& m2)
{
  return false;
}

void exportFloat32MultiArray ()
{
  using std_msgs::Float32MultiArray;
  class_<Float32MultiArray, shared_ptr<Float32MultiArray> >("Float32MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nfloat32[]         data          # array of data\n\n")
    .def_readwrite("layout", &Float32MultiArray::layout)
    .def_readwrite("data", &Float32MultiArray::data)
    ;

  class_<vector<float> >("Float32MultiArray_data")
    .def(vector_indexing_suite<vector<float> > ())
    ;
}

} // namespace
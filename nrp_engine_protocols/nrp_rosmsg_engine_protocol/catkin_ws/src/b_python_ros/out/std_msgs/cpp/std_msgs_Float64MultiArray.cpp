
#include <std_msgs/Float64MultiArray.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Float64MultiArray& m1, const Float64MultiArray& m2)
{
  return false;
}

void exportFloat64MultiArray ()
{
  using std_msgs::Float64MultiArray;
  class_<Float64MultiArray, shared_ptr<Float64MultiArray> >("Float64MultiArray", "# Please look at the MultiArrayLayout message definition for\n# documentation on all multiarrays.\n\nMultiArrayLayout  layout        # specification of data layout\nfloat64[]         data          # array of data\n\n")
    .def_readwrite("layout", &Float64MultiArray::layout)
    .def_readwrite("data", &Float64MultiArray::data)
    ;

  class_<vector<double> >("Float64MultiArray_data")
    .def(vector_indexing_suite<vector<double> > ())
    ;
}

} // namespace
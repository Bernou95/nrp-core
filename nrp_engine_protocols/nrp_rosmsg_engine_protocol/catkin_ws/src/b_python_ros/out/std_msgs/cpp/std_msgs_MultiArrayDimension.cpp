
#include <std_msgs/MultiArrayDimension.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const MultiArrayDimension& m1, const MultiArrayDimension& m2)
{
  return false;
}

void exportMultiArrayDimension ()
{
  using std_msgs::MultiArrayDimension;
  class_<MultiArrayDimension, shared_ptr<MultiArrayDimension> >("MultiArrayDimension", "string label   # label of given dimension\nuint32 size    # size of given dimension (in type units)\nuint32 stride  # stride of given dimension")
    .def_readwrite("label", &MultiArrayDimension::label)
    .def_readwrite("size", &MultiArrayDimension::size)
    .def_readwrite("stride", &MultiArrayDimension::stride)
    ;

}

} // namespace
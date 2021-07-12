
#include <std_msgs/Int32.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Int32& m1, const Int32& m2)
{
  return false;
}

void exportInt32 ()
{
  using std_msgs::Int32;
  class_<Int32, shared_ptr<Int32> >("Int32", "int32 data")
    .def_readwrite("data", &Int32::data)
    ;

}

} // namespace
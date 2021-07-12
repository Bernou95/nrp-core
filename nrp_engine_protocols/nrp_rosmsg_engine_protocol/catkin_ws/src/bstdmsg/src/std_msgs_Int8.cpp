
#include <std_msgs/Int8.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Int8& m1, const Int8& m2)
{
  return false;
}

void exportInt8 ()
{
  using std_msgs::Int8;
  class_<Int8, shared_ptr<Int8> >("Int8", "int8 data\n")
    .def_readwrite("data", &Int8::data)
    ;

}

} // namespace
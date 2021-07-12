
#include <std_msgs/Int16.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Int16& m1, const Int16& m2)
{
  return false;
}

void exportInt16 ()
{
  using std_msgs::Int16;
  class_<Int16, shared_ptr<Int16> >("Int16", "int16 data\n")
    .def_readwrite("data", &Int16::data)
    ;

}

} // namespace
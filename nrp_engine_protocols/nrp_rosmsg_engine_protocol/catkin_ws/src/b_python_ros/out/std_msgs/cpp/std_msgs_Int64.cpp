
#include <std_msgs/Int64.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Int64& m1, const Int64& m2)
{
  return false;
}

void exportInt64 ()
{
  using std_msgs::Int64;
  class_<Int64, shared_ptr<Int64> >("Int64", "int64 data")
    .def_readwrite("data", &Int64::data)
    ;

}

} // namespace
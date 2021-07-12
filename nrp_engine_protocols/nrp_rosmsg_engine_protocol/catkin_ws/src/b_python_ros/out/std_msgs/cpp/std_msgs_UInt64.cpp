
#include <std_msgs/UInt64.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const UInt64& m1, const UInt64& m2)
{
  return false;
}

void exportUInt64 ()
{
  using std_msgs::UInt64;
  class_<UInt64, shared_ptr<UInt64> >("UInt64", "uint64 data")
    .def_readwrite("data", &UInt64::data)
    ;

}

} // namespace
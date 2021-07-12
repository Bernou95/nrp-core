
#include <std_msgs/UInt32.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const UInt32& m1, const UInt32& m2)
{
  return false;
}

void exportUInt32 ()
{
  using std_msgs::UInt32;
  class_<UInt32, shared_ptr<UInt32> >("UInt32", "uint32 data")
    .def_readwrite("data", &UInt32::data)
    ;

}

} // namespace
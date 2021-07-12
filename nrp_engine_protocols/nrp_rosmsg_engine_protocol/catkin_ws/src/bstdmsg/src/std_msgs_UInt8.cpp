
#include <std_msgs/UInt8.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const UInt8& m1, const UInt8& m2)
{
  return false;
}

void exportUInt8 ()
{
  using std_msgs::UInt8;
  class_<UInt8, shared_ptr<UInt8> >("UInt8", "uint8 data\n")
    .def_readwrite("data", &UInt8::data)
    ;

}

} // namespace
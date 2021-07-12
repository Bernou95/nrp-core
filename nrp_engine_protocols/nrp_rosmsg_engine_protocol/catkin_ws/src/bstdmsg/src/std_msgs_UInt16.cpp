
#include <std_msgs/UInt16.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const UInt16& m1, const UInt16& m2)
{
  return false;
}

void exportUInt16 ()
{
  using std_msgs::UInt16;
  class_<UInt16, shared_ptr<UInt16> >("UInt16", "uint16 data\n")
    .def_readwrite("data", &UInt16::data)
    ;

}

} // namespace
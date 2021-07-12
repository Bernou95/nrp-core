
#include <std_msgs/Byte.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Byte& m1, const Byte& m2)
{
  return false;
}

void exportByte ()
{
  using std_msgs::Byte;
  class_<Byte, shared_ptr<Byte> >("Byte", "byte data\n")
    .def_readwrite("data", &Byte::data)
    ;

}

} // namespace
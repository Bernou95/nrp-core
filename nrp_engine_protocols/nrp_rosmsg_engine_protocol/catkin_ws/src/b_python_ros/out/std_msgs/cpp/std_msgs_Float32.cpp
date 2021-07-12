
#include <std_msgs/Float32.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Float32& m1, const Float32& m2)
{
  return false;
}

void exportFloat32 ()
{
  using std_msgs::Float32;
  class_<Float32, shared_ptr<Float32> >("Float32", "float32 data")
    .def_readwrite("data", &Float32::data)
    ;

}

} // namespace

#include <std_msgs/Float64.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Float64& m1, const Float64& m2)
{
  return false;
}

void exportFloat64 ()
{
  using std_msgs::Float64;
  class_<Float64, shared_ptr<Float64> >("Float64", "float64 data")
    .def_readwrite("data", &Float64::data)
    ;

}

} // namespace
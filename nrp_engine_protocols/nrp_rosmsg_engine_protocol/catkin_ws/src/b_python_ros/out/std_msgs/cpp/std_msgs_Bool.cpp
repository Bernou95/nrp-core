
#include <std_msgs/Bool.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Bool& m1, const Bool& m2)
{
  return false;
}

void exportBool ()
{
  using std_msgs::Bool;
  class_<Bool, shared_ptr<Bool> >("Bool", "bool data")
    .def_readwrite("data", &Bool::data)
    ;

}

} // namespace
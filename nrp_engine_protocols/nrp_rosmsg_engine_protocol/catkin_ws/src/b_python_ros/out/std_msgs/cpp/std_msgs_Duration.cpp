
#include <std_msgs/Duration.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Duration& m1, const Duration& m2)
{
  return false;
}

void exportDuration ()
{
  using std_msgs::Duration;
  class_<Duration, shared_ptr<Duration> >("Duration", "duration data\n")
    .def_readwrite("data", &Duration::data)
    ;

}

} // namespace

#include <std_msgs/Time.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Time& m1, const Time& m2)
{
  return false;
}

void exportTime ()
{
  using std_msgs::Time;
  class_<Time, shared_ptr<Time> >("Time", "time data\n")
    .def_readwrite("data", &Time::data)
    ;

}

} // namespace
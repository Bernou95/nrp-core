
#include <std_msgs/Empty.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Empty& m1, const Empty& m2)
{
  return false;
}

void exportEmpty ()
{
  using std_msgs::Empty;
  class_<Empty, shared_ptr<Empty> >("Empty", "")
    ;

}

} // namespace
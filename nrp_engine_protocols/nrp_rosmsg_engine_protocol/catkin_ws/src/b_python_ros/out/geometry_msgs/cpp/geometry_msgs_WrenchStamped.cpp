
#include <geometry_msgs/WrenchStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const WrenchStamped& m1, const WrenchStamped& m2)
{
  return false;
}

void exportWrenchStamped ()
{
  using geometry_msgs::WrenchStamped;
  class_<WrenchStamped, shared_ptr<WrenchStamped> >("WrenchStamped", "# A wrench with reference coordinate frame and timestamp\nHeader header\nWrench wrench\n")
    .def_readwrite("header", &WrenchStamped::header)
    .def_readwrite("wrench", &WrenchStamped::wrench)
    ;

}

} // namespace
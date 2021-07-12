
#include <geometry_msgs/InertiaStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const InertiaStamped& m1, const InertiaStamped& m2)
{
  return false;
}

void exportInertiaStamped ()
{
  using geometry_msgs::InertiaStamped;
  class_<InertiaStamped, shared_ptr<InertiaStamped> >("InertiaStamped", "Header header\nInertia inertia\n")
    .def_readwrite("header", &InertiaStamped::header)
    .def_readwrite("inertia", &InertiaStamped::inertia)
    ;

}

} // namespace

#include <geometry_msgs/Wrench.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Wrench& m1, const Wrench& m2)
{
  return false;
}

void exportWrench ()
{
  using geometry_msgs::Wrench;
  class_<Wrench, shared_ptr<Wrench> >("Wrench", "# This represents force in free space, separated into\n# its linear and angular parts.\nVector3  force\nVector3  torque\n")
    .def_readwrite("force", &Wrench::force)
    .def_readwrite("torque", &Wrench::torque)
    ;

}

} // namespace
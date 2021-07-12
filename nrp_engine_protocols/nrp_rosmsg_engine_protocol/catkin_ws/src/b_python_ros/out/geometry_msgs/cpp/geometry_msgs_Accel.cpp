
#include <geometry_msgs/Accel.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Accel& m1, const Accel& m2)
{
  return false;
}

void exportAccel ()
{
  using geometry_msgs::Accel;
  class_<Accel, shared_ptr<Accel> >("Accel", "# This expresses acceleration in free space broken into its linear and angular parts.\nVector3  linear\nVector3  angular\n")
    .def_readwrite("linear", &Accel::linear)
    .def_readwrite("angular", &Accel::angular)
    ;

}

} // namespace
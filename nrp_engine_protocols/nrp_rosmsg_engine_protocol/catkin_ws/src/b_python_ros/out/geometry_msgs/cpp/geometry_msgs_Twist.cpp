
#include <geometry_msgs/Twist.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Twist& m1, const Twist& m2)
{
  return false;
}

void exportTwist ()
{
  using geometry_msgs::Twist;
  class_<Twist, shared_ptr<Twist> >("Twist", "# This expresses velocity in free space broken into its linear and angular parts.\nVector3  linear\nVector3  angular\n")
    .def_readwrite("linear", &Twist::linear)
    .def_readwrite("angular", &Twist::angular)
    ;

}

} // namespace
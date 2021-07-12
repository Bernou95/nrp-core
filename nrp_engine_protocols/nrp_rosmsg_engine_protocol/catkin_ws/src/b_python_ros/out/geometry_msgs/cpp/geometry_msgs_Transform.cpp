
#include <geometry_msgs/Transform.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Transform& m1, const Transform& m2)
{
  return false;
}

void exportTransform ()
{
  using geometry_msgs::Transform;
  class_<Transform, shared_ptr<Transform> >("Transform", "# This represents the transform between two coordinate frames in free space.\n\nVector3 translation\nQuaternion rotation\n")
    .def_readwrite("translation", &Transform::translation)
    .def_readwrite("rotation", &Transform::rotation)
    ;

}

} // namespace
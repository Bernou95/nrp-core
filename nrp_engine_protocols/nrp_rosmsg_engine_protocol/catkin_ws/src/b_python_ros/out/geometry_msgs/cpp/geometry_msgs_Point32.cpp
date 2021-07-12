
#include <geometry_msgs/Point32.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Point32& m1, const Point32& m2)
{
  return false;
}

void exportPoint32 ()
{
  using geometry_msgs::Point32;
  class_<Point32, shared_ptr<Point32> >("Point32", "# This contains the position of a point in free space(with 32 bits of precision).\n# It is recommeded to use Point wherever possible instead of Point32.  \n# \n# This recommendation is to promote interoperability.  \n#\n# This message is designed to take up less space when sending\n# lots of points at once, as in the case of a PointCloud.  \n\nfloat32 x\nfloat32 y\nfloat32 z")
    .def_readwrite("x", &Point32::x)
    .def_readwrite("y", &Point32::y)
    .def_readwrite("z", &Point32::z)
    ;

}

} // namespace
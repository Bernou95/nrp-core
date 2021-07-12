
#include <geometry_msgs/Point.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Point& m1, const Point& m2)
{
  return false;
}

void exportPoint ()
{
  using geometry_msgs::Point;
  class_<Point, shared_ptr<Point> >("Point", "# This contains the position of a point in free space\nfloat64 x\nfloat64 y\nfloat64 z\n")
    .def_readwrite("x", &Point::x)
    .def_readwrite("y", &Point::y)
    .def_readwrite("z", &Point::z)
    ;

}

} // namespace

#include <geometry_msgs/PolygonStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const PolygonStamped& m1, const PolygonStamped& m2)
{
  return false;
}

void exportPolygonStamped ()
{
  using geometry_msgs::PolygonStamped;
  class_<PolygonStamped, shared_ptr<PolygonStamped> >("PolygonStamped", "# This represents a Polygon with reference coordinate frame and timestamp\nHeader header\nPolygon polygon\n")
    .def_readwrite("header", &PolygonStamped::header)
    .def_readwrite("polygon", &PolygonStamped::polygon)
    ;

}

} // namespace
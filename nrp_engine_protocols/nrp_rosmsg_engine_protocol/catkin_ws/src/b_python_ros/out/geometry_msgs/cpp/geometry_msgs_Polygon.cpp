
#include <geometry_msgs/Polygon.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs

{
inline bool operator== (const geometry_msgs::Point32& m1, const geometry_msgs::Point32& m2)
{
  return false;
}

} // namespace
namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Polygon& m1, const Polygon& m2)
{
  return false;
}

void exportPolygon ()
{
  using geometry_msgs::Polygon;
  class_<Polygon, shared_ptr<Polygon> >("Polygon", "#A specification of a polygon where the first and last points are assumed to be connected\nPoint32[] points\n")
    .def_readwrite("points", &Polygon::points)
    ;

  class_<vector<geometry_msgs::Point32> >("Polygon_points")
    .def(vector_indexing_suite<vector<geometry_msgs::Point32> > ())
    ;
}

} // namespace
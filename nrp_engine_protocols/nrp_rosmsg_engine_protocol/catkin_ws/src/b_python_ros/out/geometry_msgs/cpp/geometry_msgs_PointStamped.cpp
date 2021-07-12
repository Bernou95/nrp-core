
#include <geometry_msgs/PointStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const PointStamped& m1, const PointStamped& m2)
{
  return false;
}

void exportPointStamped ()
{
  using geometry_msgs::PointStamped;
  class_<PointStamped, shared_ptr<PointStamped> >("PointStamped", "# This represents a Point with reference coordinate frame and timestamp\nHeader header\nPoint point\n")
    .def_readwrite("header", &PointStamped::header)
    .def_readwrite("point", &PointStamped::point)
    ;

}

} // namespace
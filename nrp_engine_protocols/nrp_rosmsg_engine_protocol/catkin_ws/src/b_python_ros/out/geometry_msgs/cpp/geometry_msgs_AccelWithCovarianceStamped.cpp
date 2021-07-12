
#include <geometry_msgs/AccelWithCovarianceStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const AccelWithCovarianceStamped& m1, const AccelWithCovarianceStamped& m2)
{
  return false;
}

void exportAccelWithCovarianceStamped ()
{
  using geometry_msgs::AccelWithCovarianceStamped;
  class_<AccelWithCovarianceStamped, shared_ptr<AccelWithCovarianceStamped> >("AccelWithCovarianceStamped", "# This represents an estimated accel with reference coordinate frame and timestamp.\nHeader header\nAccelWithCovariance accel\n")
    .def_readwrite("header", &AccelWithCovarianceStamped::header)
    .def_readwrite("accel", &AccelWithCovarianceStamped::accel)
    ;

}

} // namespace
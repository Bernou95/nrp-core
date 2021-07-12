
#include <geometry_msgs/TwistWithCovarianceStamped.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const TwistWithCovarianceStamped& m1, const TwistWithCovarianceStamped& m2)
{
  return false;
}

void exportTwistWithCovarianceStamped ()
{
  using geometry_msgs::TwistWithCovarianceStamped;
  class_<TwistWithCovarianceStamped, shared_ptr<TwistWithCovarianceStamped> >("TwistWithCovarianceStamped", "# This represents an estimated twist with reference coordinate frame and timestamp.\nHeader header\nTwistWithCovariance twist\n")
    .def_readwrite("header", &TwistWithCovarianceStamped::header)
    .def_readwrite("twist", &TwistWithCovarianceStamped::twist)
    ;

}

} // namespace
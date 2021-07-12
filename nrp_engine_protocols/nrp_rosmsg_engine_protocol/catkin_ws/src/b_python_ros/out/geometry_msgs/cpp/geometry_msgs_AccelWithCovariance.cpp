
#include <geometry_msgs/AccelWithCovariance.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const AccelWithCovariance& m1, const AccelWithCovariance& m2)
{
  return false;
}

void exportAccelWithCovariance ()
{
  using geometry_msgs::AccelWithCovariance;
  class_<AccelWithCovariance, shared_ptr<AccelWithCovariance> >("AccelWithCovariance", "# This expresses acceleration in free space with uncertainty.\n\nAccel accel\n\n# Row-major representation of the 6x6 covariance matrix\n# The orientation parameters use a fixed-axis representation.\n# In order, the parameters are:\n# (x, y, z, rotation about X axis, rotation about Y axis, rotation about Z axis)\nfloat64[36] covariance\n")
    .def_readwrite("accel", &AccelWithCovariance::accel)
    .def_readwrite("covariance", &AccelWithCovariance::covariance)
    ;

  class_<vector<double> >("AccelWithCovariance_covariance")
    .def(vector_indexing_suite<vector<double> > ())
    ;
}

} // namespace
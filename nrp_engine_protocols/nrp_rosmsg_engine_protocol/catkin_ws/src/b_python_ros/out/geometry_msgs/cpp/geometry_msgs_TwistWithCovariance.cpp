
#include <geometry_msgs/TwistWithCovariance.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const TwistWithCovariance& m1, const TwistWithCovariance& m2)
{
  return false;
}

void exportTwistWithCovariance ()
{
  using geometry_msgs::TwistWithCovariance;
  class_<TwistWithCovariance, shared_ptr<TwistWithCovariance> >("TwistWithCovariance", "# This expresses velocity in free space with uncertainty.\n\nTwist twist\n\n# Row-major representation of the 6x6 covariance matrix\n# The orientation parameters use a fixed-axis representation.\n# In order, the parameters are:\n# (x, y, z, rotation about X axis, rotation about Y axis, rotation about Z axis)\nfloat64[36] covariance\n")
    .def_readwrite("twist", &TwistWithCovariance::twist)
    .def_readwrite("covariance", &TwistWithCovariance::covariance)
    ;

  class_<vector<double> >("TwistWithCovariance_covariance")
    .def(vector_indexing_suite<vector<double> > ())
    ;
}

} // namespace
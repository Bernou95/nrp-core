
#include <sensor_msgs/NavSatFix.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const NavSatFix& m1, const NavSatFix& m2)
{
  return false;
}

void exportNavSatFix ()
{
  using sensor_msgs::NavSatFix;
  class_<NavSatFix, shared_ptr<NavSatFix> >("NavSatFix", "# Navigation Satellite fix for any Global Navigation Satellite System\n#\n# Specified using the WGS 84 reference ellipsoid\n\n# header.stamp specifies the ROS time for this measurement (the\n#        corresponding satellite time may be reported using the\n#        sensor_msgs/TimeReference message).\n#\n# header.frame_id is the frame of reference reported by the satellite\n#        receiver, usually the location of the antenna.  This is a\n#        Euclidean frame relative to the vehicle, not a reference\n#        ellipsoid.\nHeader header\n\n# satellite fix status information\nNavSatStatus status\n\n# Latitude [degrees]. Positive is north of equator; negative is south.\nfloat64 latitude\n\n# Longitude [degrees]. Positive is east of prime meridian; negative is west.\nfloat64 longitude\n\n# Altitude [m]. Positive is above the WGS 84 ellipsoid\n# (quiet NaN if no altitude is available).\nfloat64 altitude\n\n# Position covariance [m^2] defined relative to a tangential plane\n# through the reported position. The components are East, North, and\n# Up (ENU), in row-major order.\n#\n# Beware: this coordinate system exhibits singularities at the poles.\n\nfloat64[9] position_covariance\n\n# If the covariance of the fix is known, fill it in completely. If the\n# GPS receiver provides the variance of each measurement, put them\n# along the diagonal. If only Dilution of Precision is available,\n# estimate an approximate covariance from that.\n\nuint8 COVARIANCE_TYPE_UNKNOWN = 0\nuint8 COVARIANCE_TYPE_APPROXIMATED = 1\nuint8 COVARIANCE_TYPE_DIAGONAL_KNOWN = 2\nuint8 COVARIANCE_TYPE_KNOWN = 3\n\nuint8 position_covariance_type\n")
    .def_readwrite("header", &NavSatFix::header)
    .def_readwrite("status", &NavSatFix::status)
    .def_readwrite("latitude", &NavSatFix::latitude)
    .def_readwrite("longitude", &NavSatFix::longitude)
    .def_readwrite("altitude", &NavSatFix::altitude)
    .def_readwrite("position_covariance", &NavSatFix::position_covariance)
    .def_readwrite("position_covariance_type", &NavSatFix::position_covariance_type)
    ;

  class_<vector<double> >("NavSatFix_position_covariance")
    .def(vector_indexing_suite<vector<double> > ())
    ;
}

} // namespace
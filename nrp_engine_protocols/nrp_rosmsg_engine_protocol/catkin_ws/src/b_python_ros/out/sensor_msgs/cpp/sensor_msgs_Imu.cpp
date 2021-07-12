
#include <sensor_msgs/Imu.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Imu& m1, const Imu& m2)
{
  return false;
}

void exportImu ()
{
  using sensor_msgs::Imu;
  class_<Imu, shared_ptr<Imu> >("Imu", "# This is a message to hold data from an IMU (Inertial Measurement Unit)\n#\n# Accelerations should be in m/s^2 (not in g's), and rotational velocity should be in rad/sec\n#\n# If the covariance of the measurement is known, it should be filled in (if all you know is the \n# variance of each measurement, e.g. from the datasheet, just put those along the diagonal)\n# A covariance matrix of all zeros will be interpreted as \"covariance unknown\", and to use the\n# data a covariance will have to be assumed or gotten from some other source\n#\n# If you have no estimate for one of the data elements (e.g. your IMU doesn't produce an orientation \n# estimate), please set element 0 of the associated covariance matrix to -1\n# If you are interpreting this message, please check for a value of -1 in the first element of each \n# covariance matrix, and disregard the associated estimate.\n\nHeader header\n\ngeometry_msgs/Quaternion orientation\nfloat64[9] orientation_covariance # Row major about x, y, z axes\n\ngeometry_msgs/Vector3 angular_velocity\nfloat64[9] angular_velocity_covariance # Row major about x, y, z axes\n\ngeometry_msgs/Vector3 linear_acceleration\nfloat64[9] linear_acceleration_covariance # Row major x, y z \n")
    .def_readwrite("header", &Imu::header)
    .def_readwrite("orientation", &Imu::orientation)
    .def_readwrite("orientation_covariance", &Imu::orientation_covariance)
    .def_readwrite("angular_velocity", &Imu::angular_velocity)
    .def_readwrite("angular_velocity_covariance", &Imu::angular_velocity_covariance)
    .def_readwrite("linear_acceleration", &Imu::linear_acceleration)
    .def_readwrite("linear_acceleration_covariance", &Imu::linear_acceleration_covariance)
    ;

  class_<vector<double> >("Imu_orientation_covariance")
    .def(vector_indexing_suite<vector<double> > ())
    ;
  class_<vector<double> >("Imu_angular_velocity_covariance")
    .def(vector_indexing_suite<vector<double> > ())
    ;
  class_<vector<double> >("Imu_linear_acceleration_covariance")
    .def(vector_indexing_suite<vector<double> > ())
    ;
}

} // namespace
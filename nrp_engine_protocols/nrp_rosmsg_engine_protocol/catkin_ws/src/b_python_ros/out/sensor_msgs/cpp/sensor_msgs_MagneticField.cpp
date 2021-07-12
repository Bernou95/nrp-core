
#include <sensor_msgs/MagneticField.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const MagneticField& m1, const MagneticField& m2)
{
  return false;
}

void exportMagneticField ()
{
  using sensor_msgs::MagneticField;
  class_<MagneticField, shared_ptr<MagneticField> >("MagneticField", " # Measurement of the Magnetic Field vector at a specific location.\n\n # If the covariance of the measurement is known, it should be filled in\n # (if all you know is the variance of each measurement, e.g. from the datasheet,\n #just put those along the diagonal)\n # A covariance matrix of all zeros will be interpreted as \"covariance unknown\",\n # and to use the data a covariance will have to be assumed or gotten from some\n # other source\n\n\n Header header                        # timestamp is the time the\n                                      # field was measured\n                                      # frame_id is the location and orientation\n                                      # of the field measurement\n\n geometry_msgs/Vector3 magnetic_field # x, y, and z components of the\n                                      # field vector in Tesla\n                                      # If your sensor does not output 3 axes,\n                                      # put NaNs in the components not reported.\n\n float64[9] magnetic_field_covariance # Row major about x, y, z axes\n                                      # 0 is interpreted as variance unknown")
    .def_readwrite("header", &MagneticField::header)
    .def_readwrite("magnetic_field", &MagneticField::magnetic_field)
    .def_readwrite("magnetic_field_covariance", &MagneticField::magnetic_field_covariance)
    ;

  class_<vector<double> >("MagneticField_magnetic_field_covariance")
    .def(vector_indexing_suite<vector<double> > ())
    ;
}

} // namespace
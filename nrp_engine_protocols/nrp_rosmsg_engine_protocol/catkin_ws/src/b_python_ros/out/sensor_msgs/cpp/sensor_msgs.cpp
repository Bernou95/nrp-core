
#include <boost/python.hpp>

namespace sensor_msgs
{

void exportPointCloud2();
void exportNavSatFix();
void exportIlluminance();
void exportCameraInfo();
void exportJoy();
void exportChannelFloat32();
void exportJointState();
void exportPointField();
void exportRange();
void exportMultiDOFJointState();
void exportImu();
void exportTimeReference();
void exportLaserEcho();
void exportCompressedImage();
void exportNavSatStatus();
void exportMagneticField();
void exportRelativeHumidity();
void exportRegionOfInterest();
void exportFluidPressure();
void exportBatteryState();
void exportTemperature();
void exportPointCloud();
void exportLaserScan();
void exportJoyFeedbackArray();
void exportImage();
void exportJoyFeedback();
void exportMultiEchoLaserScan();


BOOST_PYTHON_MODULE(sensor_msgs_cpp)
{
  exportPointCloud2();
  exportNavSatFix();
  exportIlluminance();
  exportCameraInfo();
  exportJoy();
  exportChannelFloat32();
  exportJointState();
  exportPointField();
  exportRange();
  exportMultiDOFJointState();
  exportImu();
  exportTimeReference();
  exportLaserEcho();
  exportCompressedImage();
  exportNavSatStatus();
  exportMagneticField();
  exportRelativeHumidity();
  exportRegionOfInterest();
  exportFluidPressure();
  exportBatteryState();
  exportTemperature();
  exportPointCloud();
  exportLaserScan();
  exportJoyFeedbackArray();
  exportImage();
  exportJoyFeedback();
  exportMultiEchoLaserScan();
}

} // namespace
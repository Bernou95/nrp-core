
#include <boost/python.hpp>

namespace std_msgs
{

void exportUInt64();
void exportUInt8MultiArray();
void exportFloat64();
void exportBool();
void exportUInt8();
void exportColorRGBA();
void exportByteMultiArray();
void exportInt64();
void exportInt32();
void exportMultiArrayDimension();
void exportInt16MultiArray();
void exportFloat32MultiArray();
void exportChar();
void exportInt8();
void exportUInt16MultiArray();
void exportEmpty();
void exportString();
void exportUInt16();
void exportInt8MultiArray();
void exportMultiArrayLayout();
void exportUInt32MultiArray();
void exportFloat32();
void exportInt16();
void exportInt32MultiArray();
void exportHeader();
void exportDuration();
void exportTime();
void exportInt64MultiArray();
void exportUInt64MultiArray();
void exportUInt32();
void exportFloat64MultiArray();
void exportByte();


BOOST_PYTHON_MODULE(std_msgs_cpp)
{
  exportUInt64();
  exportUInt8MultiArray();
  exportFloat64();
  exportBool();
  exportUInt8();
  exportColorRGBA();
  exportByteMultiArray();
  exportInt64();
  exportInt32();
  exportMultiArrayDimension();
  exportInt16MultiArray();
  exportFloat32MultiArray();
  exportChar();
  exportInt8();
  exportUInt16MultiArray();
  exportEmpty();
  exportString();
  exportUInt16();
  exportInt8MultiArray();
  exportMultiArrayLayout();
  exportUInt32MultiArray();
  exportFloat32();
  exportInt16();
  exportInt32MultiArray();
  exportHeader();
  exportDuration();
  exportTime();
  exportInt64MultiArray();
  exportUInt64MultiArray();
  exportUInt32();
  exportFloat64MultiArray();
  exportByte();
}

} // namespace
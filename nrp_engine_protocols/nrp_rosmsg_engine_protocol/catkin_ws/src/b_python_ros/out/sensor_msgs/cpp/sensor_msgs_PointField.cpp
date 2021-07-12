
#include <sensor_msgs/PointField.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const PointField& m1, const PointField& m2)
{
  return false;
}

void exportPointField ()
{
  using sensor_msgs::PointField;
  class_<PointField, shared_ptr<PointField> >("PointField", "# This message holds the description of one point entry in the\n# PointCloud2 message format.\nuint8 INT8    = 1\nuint8 UINT8   = 2\nuint8 INT16   = 3\nuint8 UINT16  = 4\nuint8 INT32   = 5\nuint8 UINT32  = 6\nuint8 FLOAT32 = 7\nuint8 FLOAT64 = 8\n\nstring name      # Name of field\nuint32 offset    # Offset from start of point struct\nuint8  datatype  # Datatype enumeration, see above\nuint32 count     # How many elements in the field\n")
    .def_readwrite("name", &PointField::name)
    .def_readwrite("offset", &PointField::offset)
    .def_readwrite("datatype", &PointField::datatype)
    .def_readwrite("count", &PointField::count)
    ;

}

} // namespace

#include <nav_msgs/MapMetaData.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace nav_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const MapMetaData& m1, const MapMetaData& m2)
{
  return false;
}

void exportMapMetaData ()
{
  using nav_msgs::MapMetaData;
  class_<MapMetaData, shared_ptr<MapMetaData> >("MapMetaData", "# This hold basic information about the characterists of the OccupancyGrid\n\n# The time at which the map was loaded\ntime map_load_time\n# The map resolution [m/cell]\nfloat32 resolution\n# Map width [cells]\nuint32 width\n# Map height [cells]\nuint32 height\n# The origin of the map [m, m, rad].  This is the real-world pose of the\n# cell (0,0) in the map.\ngeometry_msgs/Pose origin")
    .def_readwrite("map_load_time", &MapMetaData::map_load_time)
    .def_readwrite("resolution", &MapMetaData::resolution)
    .def_readwrite("width", &MapMetaData::width)
    .def_readwrite("height", &MapMetaData::height)
    .def_readwrite("origin", &MapMetaData::origin)
    ;

}

} // namespace
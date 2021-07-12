
#include <nav_msgs/OccupancyGrid.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace nav_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const OccupancyGrid& m1, const OccupancyGrid& m2)
{
  return false;
}

void exportOccupancyGrid ()
{
  using nav_msgs::OccupancyGrid;
  class_<OccupancyGrid, shared_ptr<OccupancyGrid> >("OccupancyGrid", "# This represents a 2-D grid map, in which each cell represents the probability of\n# occupancy.\n\nHeader header \n\n#MetaData for the map\nMapMetaData info\n\n# The map data, in row-major order, starting with (0,0).  Occupancy\n# probabilities are in the range [0,100].  Unknown is -1.\nint8[] data\n")
    .def_readwrite("header", &OccupancyGrid::header)
    .def_readwrite("info", &OccupancyGrid::info)
    .def_readwrite("data", &OccupancyGrid::data)
    ;

  class_<vector<int8_t> >("OccupancyGrid_data")
    .def(vector_indexing_suite<vector<int8_t> > ())
    ;
}

} // namespace

#include <nav_msgs/GridCells.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs

{
inline bool operator== (const geometry_msgs::Point& m1, const geometry_msgs::Point& m2)
{
  return false;
}

} // namespace
namespace nav_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const GridCells& m1, const GridCells& m2)
{
  return false;
}

void exportGridCells ()
{
  using nav_msgs::GridCells;
  class_<GridCells, shared_ptr<GridCells> >("GridCells", "#an array of cells in a 2D grid\nHeader header\nfloat32 cell_width\nfloat32 cell_height\ngeometry_msgs/Point[] cells\n")
    .def_readwrite("header", &GridCells::header)
    .def_readwrite("cell_width", &GridCells::cell_width)
    .def_readwrite("cell_height", &GridCells::cell_height)
    .def_readwrite("cells", &GridCells::cells)
    ;

  class_<vector<geometry_msgs::Point> >("GridCells_cells")
    .def(vector_indexing_suite<vector<geometry_msgs::Point> > ())
    ;
}

} // namespace
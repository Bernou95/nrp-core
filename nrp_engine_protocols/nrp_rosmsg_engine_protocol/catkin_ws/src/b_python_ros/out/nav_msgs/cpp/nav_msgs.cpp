
#include <boost/python.hpp>

namespace nav_msgs
{

void exportMapMetaData();
void exportOdometry();
void exportGetMapGoal();
void exportGetMapActionGoal();
void exportGetMapAction();
void exportGetMapActionResult();
void exportGetMapFeedback();
void exportGridCells();
void exportGetMapResult();
void exportGetMapActionFeedback();
void exportOccupancyGrid();
void exportPath();


BOOST_PYTHON_MODULE(nav_msgs_cpp)
{
  exportMapMetaData();
  exportOdometry();
  exportGetMapGoal();
  exportGetMapActionGoal();
  exportGetMapAction();
  exportGetMapActionResult();
  exportGetMapFeedback();
  exportGridCells();
  exportGetMapResult();
  exportGetMapActionFeedback();
  exportOccupancyGrid();
  exportPath();
}

} // namespace
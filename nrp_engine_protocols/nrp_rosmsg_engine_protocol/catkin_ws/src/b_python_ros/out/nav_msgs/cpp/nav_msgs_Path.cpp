
#include <nav_msgs/Path.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace geometry_msgs

{
inline bool operator== (const geometry_msgs::PoseStamped& m1, const geometry_msgs::PoseStamped& m2)
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
bool operator== (const Path& m1, const Path& m2)
{
  return false;
}

void exportPath ()
{
  using nav_msgs::Path;
  class_<Path, shared_ptr<Path> >("Path", "#An array of poses that represents a Path for a robot to follow\nHeader header\ngeometry_msgs/PoseStamped[] poses\n")
    .def_readwrite("header", &Path::header)
    .def_readwrite("poses", &Path::poses)
    ;

  class_<vector<geometry_msgs::PoseStamped> >("Path_poses")
    .def(vector_indexing_suite<vector<geometry_msgs::PoseStamped> > ())
    ;
}

} // namespace
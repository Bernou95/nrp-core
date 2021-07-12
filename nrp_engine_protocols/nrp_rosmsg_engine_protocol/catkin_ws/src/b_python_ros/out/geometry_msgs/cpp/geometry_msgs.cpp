
#include <boost/python.hpp>

namespace geometry_msgs
{

void exportAccel();
void exportTwistStamped();
void exportTwistWithCovarianceStamped();
void exportPolygonStamped();
void exportInertiaStamped();
void exportPoseWithCovariance();
void exportTransform();
void exportPointStamped();
void exportAccelWithCovariance();
void exportVector3Stamped();
void exportPoseStamped();
void exportPose2D();
void exportInertia();
void exportTransformStamped();
void exportWrench();
void exportWrenchStamped();
void exportTwistWithCovariance();
void exportPoseArray();
void exportAccelStamped();
void exportPoint32();
void exportPoint();
void exportPoseWithCovarianceStamped();
void exportPose();
void exportTwist();
void exportQuaternion();
void exportVector3();
void exportPolygon();
void exportAccelWithCovarianceStamped();
void exportQuaternionStamped();


BOOST_PYTHON_MODULE(geometry_msgs_cpp)
{
  exportAccel();
  exportTwistStamped();
  exportTwistWithCovarianceStamped();
  exportPolygonStamped();
  exportInertiaStamped();
  exportPoseWithCovariance();
  exportTransform();
  exportPointStamped();
  exportAccelWithCovariance();
  exportVector3Stamped();
  exportPoseStamped();
  exportPose2D();
  exportInertia();
  exportTransformStamped();
  exportWrench();
  exportWrenchStamped();
  exportTwistWithCovariance();
  exportPoseArray();
  exportAccelStamped();
  exportPoint32();
  exportPoint();
  exportPoseWithCovarianceStamped();
  exportPose();
  exportTwist();
  exportQuaternion();
  exportVector3();
  exportPolygon();
  exportAccelWithCovarianceStamped();
  exportQuaternionStamped();
}

} // namespace
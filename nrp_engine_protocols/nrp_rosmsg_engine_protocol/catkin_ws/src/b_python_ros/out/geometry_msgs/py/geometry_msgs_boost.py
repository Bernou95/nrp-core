import roslib; roslib.load_manifest('None')
import rostime_boost_python
import std_msgs_boost
import geometry_msgs.msg
import geometry_msgs_cpp as cpp

def Accel_to_ros(x):
    m = geometry_msgs.msg.Accel()
    m.linear = x.linear.to_ros()
    m.angular = x.angular.to_ros()
    return m

cpp.Accel.to_ros = Accel_to_ros

def Accel_to_boost(m):
    x = cpp.Accel()
    x.linear = m.linear.to_boost()
    x.angular = m.angular.to_boost()
    return x

geometry_msgs.msg.Accel.to_boost = Accel_to_boost

def TwistStamped_to_ros(x):
    m = geometry_msgs.msg.TwistStamped()
    m.header = x.header.to_ros()
    m.twist = x.twist.to_ros()
    return m

cpp.TwistStamped.to_ros = TwistStamped_to_ros

def TwistStamped_to_boost(m):
    x = cpp.TwistStamped()
    x.header = m.header.to_boost()
    x.twist = m.twist.to_boost()
    return x

geometry_msgs.msg.TwistStamped.to_boost = TwistStamped_to_boost

def TwistWithCovarianceStamped_to_ros(x):
    m = geometry_msgs.msg.TwistWithCovarianceStamped()
    m.header = x.header.to_ros()
    m.twist = x.twist.to_ros()
    return m

cpp.TwistWithCovarianceStamped.to_ros = TwistWithCovarianceStamped_to_ros

def TwistWithCovarianceStamped_to_boost(m):
    x = cpp.TwistWithCovarianceStamped()
    x.header = m.header.to_boost()
    x.twist = m.twist.to_boost()
    return x

geometry_msgs.msg.TwistWithCovarianceStamped.to_boost = TwistWithCovarianceStamped_to_boost

def PolygonStamped_to_ros(x):
    m = geometry_msgs.msg.PolygonStamped()
    m.header = x.header.to_ros()
    m.polygon = x.polygon.to_ros()
    return m

cpp.PolygonStamped.to_ros = PolygonStamped_to_ros

def PolygonStamped_to_boost(m):
    x = cpp.PolygonStamped()
    x.header = m.header.to_boost()
    x.polygon = m.polygon.to_boost()
    return x

geometry_msgs.msg.PolygonStamped.to_boost = PolygonStamped_to_boost

def InertiaStamped_to_ros(x):
    m = geometry_msgs.msg.InertiaStamped()
    m.header = x.header.to_ros()
    m.inertia = x.inertia.to_ros()
    return m

cpp.InertiaStamped.to_ros = InertiaStamped_to_ros

def InertiaStamped_to_boost(m):
    x = cpp.InertiaStamped()
    x.header = m.header.to_boost()
    x.inertia = m.inertia.to_boost()
    return x

geometry_msgs.msg.InertiaStamped.to_boost = InertiaStamped_to_boost

def PoseWithCovariance_to_ros(x):
    m = geometry_msgs.msg.PoseWithCovariance()
    m.pose = x.pose.to_ros()
    m.covariance = x.covariance
    return m

cpp.PoseWithCovariance.to_ros = PoseWithCovariance_to_ros

def PoseWithCovariance_to_boost(m):
    x = cpp.PoseWithCovariance()
    x.pose = m.pose.to_boost()
    x.covariance = m.covariance
    return x

geometry_msgs.msg.PoseWithCovariance.to_boost = PoseWithCovariance_to_boost

def Transform_to_ros(x):
    m = geometry_msgs.msg.Transform()
    m.translation = x.translation.to_ros()
    m.rotation = x.rotation.to_ros()
    return m

cpp.Transform.to_ros = Transform_to_ros

def Transform_to_boost(m):
    x = cpp.Transform()
    x.translation = m.translation.to_boost()
    x.rotation = m.rotation.to_boost()
    return x

geometry_msgs.msg.Transform.to_boost = Transform_to_boost

def PointStamped_to_ros(x):
    m = geometry_msgs.msg.PointStamped()
    m.header = x.header.to_ros()
    m.point = x.point.to_ros()
    return m

cpp.PointStamped.to_ros = PointStamped_to_ros

def PointStamped_to_boost(m):
    x = cpp.PointStamped()
    x.header = m.header.to_boost()
    x.point = m.point.to_boost()
    return x

geometry_msgs.msg.PointStamped.to_boost = PointStamped_to_boost

def AccelWithCovariance_to_ros(x):
    m = geometry_msgs.msg.AccelWithCovariance()
    m.accel = x.accel.to_ros()
    m.covariance = x.covariance
    return m

cpp.AccelWithCovariance.to_ros = AccelWithCovariance_to_ros

def AccelWithCovariance_to_boost(m):
    x = cpp.AccelWithCovariance()
    x.accel = m.accel.to_boost()
    x.covariance = m.covariance
    return x

geometry_msgs.msg.AccelWithCovariance.to_boost = AccelWithCovariance_to_boost

def Vector3Stamped_to_ros(x):
    m = geometry_msgs.msg.Vector3Stamped()
    m.header = x.header.to_ros()
    m.vector = x.vector.to_ros()
    return m

cpp.Vector3Stamped.to_ros = Vector3Stamped_to_ros

def Vector3Stamped_to_boost(m):
    x = cpp.Vector3Stamped()
    x.header = m.header.to_boost()
    x.vector = m.vector.to_boost()
    return x

geometry_msgs.msg.Vector3Stamped.to_boost = Vector3Stamped_to_boost

def PoseStamped_to_ros(x):
    m = geometry_msgs.msg.PoseStamped()
    m.header = x.header.to_ros()
    m.pose = x.pose.to_ros()
    return m

cpp.PoseStamped.to_ros = PoseStamped_to_ros

def PoseStamped_to_boost(m):
    x = cpp.PoseStamped()
    x.header = m.header.to_boost()
    x.pose = m.pose.to_boost()
    return x

geometry_msgs.msg.PoseStamped.to_boost = PoseStamped_to_boost

def Pose2D_to_ros(x):
    m = geometry_msgs.msg.Pose2D()
    m.x = x.x
    m.y = x.y
    m.theta = x.theta
    return m

cpp.Pose2D.to_ros = Pose2D_to_ros

def Pose2D_to_boost(m):
    x = cpp.Pose2D()
    x.x = m.x
    x.y = m.y
    x.theta = m.theta
    return x

geometry_msgs.msg.Pose2D.to_boost = Pose2D_to_boost

def Inertia_to_ros(x):
    m = geometry_msgs.msg.Inertia()
    m.m = x.m
    m.com = x.com.to_ros()
    m.ixx = x.ixx
    m.ixy = x.ixy
    m.ixz = x.ixz
    m.iyy = x.iyy
    m.iyz = x.iyz
    m.izz = x.izz
    return m

cpp.Inertia.to_ros = Inertia_to_ros

def Inertia_to_boost(m):
    x = cpp.Inertia()
    x.m = m.m
    x.com = m.com.to_boost()
    x.ixx = m.ixx
    x.ixy = m.ixy
    x.ixz = m.ixz
    x.iyy = m.iyy
    x.iyz = m.iyz
    x.izz = m.izz
    return x

geometry_msgs.msg.Inertia.to_boost = Inertia_to_boost

def TransformStamped_to_ros(x):
    m = geometry_msgs.msg.TransformStamped()
    m.header = x.header.to_ros()
    m.child_frame_id = x.child_frame_id
    m.transform = x.transform.to_ros()
    return m

cpp.TransformStamped.to_ros = TransformStamped_to_ros

def TransformStamped_to_boost(m):
    x = cpp.TransformStamped()
    x.header = m.header.to_boost()
    x.child_frame_id = m.child_frame_id
    x.transform = m.transform.to_boost()
    return x

geometry_msgs.msg.TransformStamped.to_boost = TransformStamped_to_boost

def Wrench_to_ros(x):
    m = geometry_msgs.msg.Wrench()
    m.force = x.force.to_ros()
    m.torque = x.torque.to_ros()
    return m

cpp.Wrench.to_ros = Wrench_to_ros

def Wrench_to_boost(m):
    x = cpp.Wrench()
    x.force = m.force.to_boost()
    x.torque = m.torque.to_boost()
    return x

geometry_msgs.msg.Wrench.to_boost = Wrench_to_boost

def WrenchStamped_to_ros(x):
    m = geometry_msgs.msg.WrenchStamped()
    m.header = x.header.to_ros()
    m.wrench = x.wrench.to_ros()
    return m

cpp.WrenchStamped.to_ros = WrenchStamped_to_ros

def WrenchStamped_to_boost(m):
    x = cpp.WrenchStamped()
    x.header = m.header.to_boost()
    x.wrench = m.wrench.to_boost()
    return x

geometry_msgs.msg.WrenchStamped.to_boost = WrenchStamped_to_boost

def TwistWithCovariance_to_ros(x):
    m = geometry_msgs.msg.TwistWithCovariance()
    m.twist = x.twist.to_ros()
    m.covariance = x.covariance
    return m

cpp.TwistWithCovariance.to_ros = TwistWithCovariance_to_ros

def TwistWithCovariance_to_boost(m):
    x = cpp.TwistWithCovariance()
    x.twist = m.twist.to_boost()
    x.covariance = m.covariance
    return x

geometry_msgs.msg.TwistWithCovariance.to_boost = TwistWithCovariance_to_boost

def PoseArray_to_ros(x):
    m = geometry_msgs.msg.PoseArray()
    m.header = x.header.to_ros()
    m.poses = [y.to_ros() for y in x.poses]
    return m

cpp.PoseArray.to_ros = PoseArray_to_ros

def PoseArray_to_boost(m):
    x = cpp.PoseArray()
    x.header = m.header.to_boost()
    x.poses[:] = [y.to_boost() for y in m.poses]
    return x

geometry_msgs.msg.PoseArray.to_boost = PoseArray_to_boost

def AccelStamped_to_ros(x):
    m = geometry_msgs.msg.AccelStamped()
    m.header = x.header.to_ros()
    m.accel = x.accel.to_ros()
    return m

cpp.AccelStamped.to_ros = AccelStamped_to_ros

def AccelStamped_to_boost(m):
    x = cpp.AccelStamped()
    x.header = m.header.to_boost()
    x.accel = m.accel.to_boost()
    return x

geometry_msgs.msg.AccelStamped.to_boost = AccelStamped_to_boost

def Point32_to_ros(x):
    m = geometry_msgs.msg.Point32()
    m.x = x.x
    m.y = x.y
    m.z = x.z
    return m

cpp.Point32.to_ros = Point32_to_ros

def Point32_to_boost(m):
    x = cpp.Point32()
    x.x = m.x
    x.y = m.y
    x.z = m.z
    return x

geometry_msgs.msg.Point32.to_boost = Point32_to_boost

def Point_to_ros(x):
    m = geometry_msgs.msg.Point()
    m.x = x.x
    m.y = x.y
    m.z = x.z
    return m

cpp.Point.to_ros = Point_to_ros

def Point_to_boost(m):
    x = cpp.Point()
    x.x = m.x
    x.y = m.y
    x.z = m.z
    return x

geometry_msgs.msg.Point.to_boost = Point_to_boost

def PoseWithCovarianceStamped_to_ros(x):
    m = geometry_msgs.msg.PoseWithCovarianceStamped()
    m.header = x.header.to_ros()
    m.pose = x.pose.to_ros()
    return m

cpp.PoseWithCovarianceStamped.to_ros = PoseWithCovarianceStamped_to_ros

def PoseWithCovarianceStamped_to_boost(m):
    x = cpp.PoseWithCovarianceStamped()
    x.header = m.header.to_boost()
    x.pose = m.pose.to_boost()
    return x

geometry_msgs.msg.PoseWithCovarianceStamped.to_boost = PoseWithCovarianceStamped_to_boost

def Pose_to_ros(x):
    m = geometry_msgs.msg.Pose()
    m.position = x.position.to_ros()
    m.orientation = x.orientation.to_ros()
    return m

cpp.Pose.to_ros = Pose_to_ros

def Pose_to_boost(m):
    x = cpp.Pose()
    x.position = m.position.to_boost()
    x.orientation = m.orientation.to_boost()
    return x

geometry_msgs.msg.Pose.to_boost = Pose_to_boost

def Twist_to_ros(x):
    m = geometry_msgs.msg.Twist()
    m.linear = x.linear.to_ros()
    m.angular = x.angular.to_ros()
    return m

cpp.Twist.to_ros = Twist_to_ros

def Twist_to_boost(m):
    x = cpp.Twist()
    x.linear = m.linear.to_boost()
    x.angular = m.angular.to_boost()
    return x

geometry_msgs.msg.Twist.to_boost = Twist_to_boost

def Quaternion_to_ros(x):
    m = geometry_msgs.msg.Quaternion()
    m.x = x.x
    m.y = x.y
    m.z = x.z
    m.w = x.w
    return m

cpp.Quaternion.to_ros = Quaternion_to_ros

def Quaternion_to_boost(m):
    x = cpp.Quaternion()
    x.x = m.x
    x.y = m.y
    x.z = m.z
    x.w = m.w
    return x

geometry_msgs.msg.Quaternion.to_boost = Quaternion_to_boost

def Vector3_to_ros(x):
    m = geometry_msgs.msg.Vector3()
    m.x = x.x
    m.y = x.y
    m.z = x.z
    return m

cpp.Vector3.to_ros = Vector3_to_ros

def Vector3_to_boost(m):
    x = cpp.Vector3()
    x.x = m.x
    x.y = m.y
    x.z = m.z
    return x

geometry_msgs.msg.Vector3.to_boost = Vector3_to_boost

def Polygon_to_ros(x):
    m = geometry_msgs.msg.Polygon()
    m.points = [y.to_ros() for y in x.points]
    return m

cpp.Polygon.to_ros = Polygon_to_ros

def Polygon_to_boost(m):
    x = cpp.Polygon()
    x.points[:] = [y.to_boost() for y in m.points]
    return x

geometry_msgs.msg.Polygon.to_boost = Polygon_to_boost

def AccelWithCovarianceStamped_to_ros(x):
    m = geometry_msgs.msg.AccelWithCovarianceStamped()
    m.header = x.header.to_ros()
    m.accel = x.accel.to_ros()
    return m

cpp.AccelWithCovarianceStamped.to_ros = AccelWithCovarianceStamped_to_ros

def AccelWithCovarianceStamped_to_boost(m):
    x = cpp.AccelWithCovarianceStamped()
    x.header = m.header.to_boost()
    x.accel = m.accel.to_boost()
    return x

geometry_msgs.msg.AccelWithCovarianceStamped.to_boost = AccelWithCovarianceStamped_to_boost

def QuaternionStamped_to_ros(x):
    m = geometry_msgs.msg.QuaternionStamped()
    m.header = x.header.to_ros()
    m.quaternion = x.quaternion.to_ros()
    return m

cpp.QuaternionStamped.to_ros = QuaternionStamped_to_ros

def QuaternionStamped_to_boost(m):
    x = cpp.QuaternionStamped()
    x.header = m.header.to_boost()
    x.quaternion = m.quaternion.to_boost()
    return x

geometry_msgs.msg.QuaternionStamped.to_boost = QuaternionStamped_to_boost

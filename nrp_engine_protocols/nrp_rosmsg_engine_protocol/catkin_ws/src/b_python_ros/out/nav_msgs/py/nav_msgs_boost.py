import roslib; roslib.load_manifest('None')
import rostime_boost_python
import geometry_msgs_boost
import std_msgs_boost
import actionlib_msgs_boost
import nav_msgs.msg
import nav_msgs_cpp as cpp

def MapMetaData_to_ros(x):
    m = nav_msgs.msg.MapMetaData()
    m.map_load_time.secs = x.map_load_time.sec
    m.map_load_time.nsecs = x.map_load_time.nsec
    m.resolution = x.resolution
    m.width = x.width
    m.height = x.height
    m.origin = x.origin.to_ros()
    return m

cpp.MapMetaData.to_ros = MapMetaData_to_ros

def MapMetaData_to_boost(m):
    x = cpp.MapMetaData()
    x.map_load_time.sec = m.map_load_time.secs
    x.map_load_time.nsec = m.map_load_time.nsecs
    x.resolution = m.resolution
    x.width = m.width
    x.height = m.height
    x.origin = m.origin.to_boost()
    return x

nav_msgs.msg.MapMetaData.to_boost = MapMetaData_to_boost

def Odometry_to_ros(x):
    m = nav_msgs.msg.Odometry()
    m.header = x.header.to_ros()
    m.child_frame_id = x.child_frame_id
    m.pose = x.pose.to_ros()
    m.twist = x.twist.to_ros()
    return m

cpp.Odometry.to_ros = Odometry_to_ros

def Odometry_to_boost(m):
    x = cpp.Odometry()
    x.header = m.header.to_boost()
    x.child_frame_id = m.child_frame_id
    x.pose = m.pose.to_boost()
    x.twist = m.twist.to_boost()
    return x

nav_msgs.msg.Odometry.to_boost = Odometry_to_boost

def GetMapGoal_to_ros(x):
    m = nav_msgs.msg.GetMapGoal()
    return m

cpp.GetMapGoal.to_ros = GetMapGoal_to_ros

def GetMapGoal_to_boost(m):
    x = cpp.GetMapGoal()
    return x

nav_msgs.msg.GetMapGoal.to_boost = GetMapGoal_to_boost

def GetMapActionGoal_to_ros(x):
    m = nav_msgs.msg.GetMapActionGoal()
    m.header = x.header.to_ros()
    m.goal_id = x.goal_id.to_ros()
    m.goal = x.goal.to_ros()
    return m

cpp.GetMapActionGoal.to_ros = GetMapActionGoal_to_ros

def GetMapActionGoal_to_boost(m):
    x = cpp.GetMapActionGoal()
    x.header = m.header.to_boost()
    x.goal_id = m.goal_id.to_boost()
    x.goal = m.goal.to_boost()
    return x

nav_msgs.msg.GetMapActionGoal.to_boost = GetMapActionGoal_to_boost

def GetMapAction_to_ros(x):
    m = nav_msgs.msg.GetMapAction()
    m.action_goal = x.action_goal.to_ros()
    m.action_result = x.action_result.to_ros()
    m.action_feedback = x.action_feedback.to_ros()
    return m

cpp.GetMapAction.to_ros = GetMapAction_to_ros

def GetMapAction_to_boost(m):
    x = cpp.GetMapAction()
    x.action_goal = m.action_goal.to_boost()
    x.action_result = m.action_result.to_boost()
    x.action_feedback = m.action_feedback.to_boost()
    return x

nav_msgs.msg.GetMapAction.to_boost = GetMapAction_to_boost

def GetMapActionResult_to_ros(x):
    m = nav_msgs.msg.GetMapActionResult()
    m.header = x.header.to_ros()
    m.status = x.status.to_ros()
    m.result = x.result.to_ros()
    return m

cpp.GetMapActionResult.to_ros = GetMapActionResult_to_ros

def GetMapActionResult_to_boost(m):
    x = cpp.GetMapActionResult()
    x.header = m.header.to_boost()
    x.status = m.status.to_boost()
    x.result = m.result.to_boost()
    return x

nav_msgs.msg.GetMapActionResult.to_boost = GetMapActionResult_to_boost

def GetMapFeedback_to_ros(x):
    m = nav_msgs.msg.GetMapFeedback()
    return m

cpp.GetMapFeedback.to_ros = GetMapFeedback_to_ros

def GetMapFeedback_to_boost(m):
    x = cpp.GetMapFeedback()
    return x

nav_msgs.msg.GetMapFeedback.to_boost = GetMapFeedback_to_boost

def GridCells_to_ros(x):
    m = nav_msgs.msg.GridCells()
    m.header = x.header.to_ros()
    m.cell_width = x.cell_width
    m.cell_height = x.cell_height
    m.cells = [y.to_ros() for y in x.cells]
    return m

cpp.GridCells.to_ros = GridCells_to_ros

def GridCells_to_boost(m):
    x = cpp.GridCells()
    x.header = m.header.to_boost()
    x.cell_width = m.cell_width
    x.cell_height = m.cell_height
    x.cells[:] = [y.to_boost() for y in m.cells]
    return x

nav_msgs.msg.GridCells.to_boost = GridCells_to_boost

def GetMapResult_to_ros(x):
    m = nav_msgs.msg.GetMapResult()
    m.map = x.map.to_ros()
    return m

cpp.GetMapResult.to_ros = GetMapResult_to_ros

def GetMapResult_to_boost(m):
    x = cpp.GetMapResult()
    x.map = m.map.to_boost()
    return x

nav_msgs.msg.GetMapResult.to_boost = GetMapResult_to_boost

def GetMapActionFeedback_to_ros(x):
    m = nav_msgs.msg.GetMapActionFeedback()
    m.header = x.header.to_ros()
    m.status = x.status.to_ros()
    m.feedback = x.feedback.to_ros()
    return m

cpp.GetMapActionFeedback.to_ros = GetMapActionFeedback_to_ros

def GetMapActionFeedback_to_boost(m):
    x = cpp.GetMapActionFeedback()
    x.header = m.header.to_boost()
    x.status = m.status.to_boost()
    x.feedback = m.feedback.to_boost()
    return x

nav_msgs.msg.GetMapActionFeedback.to_boost = GetMapActionFeedback_to_boost

def OccupancyGrid_to_ros(x):
    m = nav_msgs.msg.OccupancyGrid()
    m.header = x.header.to_ros()
    m.info = x.info.to_ros()
    m.data = x.data
    return m

cpp.OccupancyGrid.to_ros = OccupancyGrid_to_ros

def OccupancyGrid_to_boost(m):
    x = cpp.OccupancyGrid()
    x.header = m.header.to_boost()
    x.info = m.info.to_boost()
    x.data = m.data
    return x

nav_msgs.msg.OccupancyGrid.to_boost = OccupancyGrid_to_boost

def Path_to_ros(x):
    m = nav_msgs.msg.Path()
    m.header = x.header.to_ros()
    m.poses = [y.to_ros() for y in x.poses]
    return m

cpp.Path.to_ros = Path_to_ros

def Path_to_boost(m):
    x = cpp.Path()
    x.header = m.header.to_boost()
    x.poses[:] = [y.to_boost() for y in m.poses]
    return x

nav_msgs.msg.Path.to_boost = Path_to_boost

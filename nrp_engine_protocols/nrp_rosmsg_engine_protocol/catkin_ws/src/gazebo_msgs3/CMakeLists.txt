cmake_minimum_required(VERSION 2.8.3)
project(gazebo_msgs3)

find_package(catkin REQUIRED COMPONENTS
  std_msgs
  nav_msgs
  trajectory_msgs
  geometry_msgs
  sensor_msgs
  std_srvs
  message_generation
  )

add_message_files(
  #If you add messages using DIRECTORY, for some reason, header files are not generated
  #which causes an error when boost_python_ros tries to generate the python bindings  
  #DIRECTORY msg
  
  FILES
  ContactState.msg
  JointStates.msg
  OpenSimPhysics.msg
  WheelSpeeds.msg
  )



generate_messages(DEPENDENCIES
  std_msgs
  geometry_msgs
  nav_msgs
  sensor_msgs
  trajectory_msgs
  )

catkin_package(
  CATKIN_DEPENDS
  message_runtime
  std_msgs
  nav_msgs
  trajectory_msgs
  geometry_msgs
  sensor_msgs
  std_srvs
  )

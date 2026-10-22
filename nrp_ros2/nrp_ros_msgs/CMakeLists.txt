cmake_minimum_required(VERSION 3.16)
project(nrp_ros_msgs VERSION 1.0)

##########################################
## Dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

##########################################
## Generate msgs
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Test.msg"
  "msg/SpikeGroup.msg"
  "msg/FullStateCompactDelay.msg"
  DEPENDENCIES builtin_interfaces std_msgs geometry_msgs
  )

ament_export_dependencies(rosidl_default_runtime)
ament_package()

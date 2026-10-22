cmake_minimum_required(VERSION 3.16)
project(NRPROS2 VERSION ${NRP_VERSION})

if("$ENV{ROS_DISTRO}" STREQUAL "" OR "$ENV{AMENT_PREFIX_PATH}" STREQUAL "")
    message(FATAL_ERROR "ROS 2 environment not found. Source /opt/ros/<distro>/setup.bash before running cmake")
endif()
message(STATUS "ROS 2 distribution: $ENV{ROS_DISTRO}")

##########################################
## Copy the nrp message definitions to a colcon workspace which path is specified by var ROS2_WS
## and build that workspace
set(ROS2_WS $ENV{ROS2_WS})
if ("${ROS2_WS}" STREQUAL "")
    message(STATUS "Setting ros2_ws path to: $ENV{HOME}/ros2_ws")
    set(ROS2_WS $ENV{HOME}/ros2_ws)
endif()

if (NOT EXISTS ${ROS2_WS}/src)
    message(STATUS "Creating ros2_ws folder: ${ROS2_WS}")
    file(MAKE_DIRECTORY ${ROS2_WS}/src)
endif()

file(REMOVE_RECURSE ${ROS2_WS}/src/nrp_ros_msgs)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/nrp_ros_msgs DESTINATION ${ROS2_WS}/src)

execute_process(
        WORKING_DIRECTORY ${ROS2_WS}
        COMMAND colcon build --packages-select nrp_ros_msgs
        RESULT_VARIABLE COLCON_RESULT
        OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/colcon_build.log
        ERROR_FILE ${CMAKE_CURRENT_BINARY_DIR}/colcon_build.log
)
if(NOT COLCON_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to build nrp_ros_msgs in ${ROS2_WS}. See ${CMAKE_CURRENT_BINARY_DIR}/colcon_build.log")
endif()

set(NRP_ROS2_MSGS_INSTALL_DIR "${ROS2_WS}/install/nrp_ros_msgs" CACHE INTERNAL "Install directory of the nrp_ros_msgs ROS 2 package")
set(ENV{AMENT_PREFIX_PATH} "${NRP_ROS2_MSGS_INSTALL_DIR}:$ENV{AMENT_PREFIX_PATH}")
list(APPEND CMAKE_PREFIX_PATH ${NRP_ROS2_MSGS_INSTALL_DIR})

##########################################
## Build nrp ros python bindings
add_subdirectory(python_bindings)

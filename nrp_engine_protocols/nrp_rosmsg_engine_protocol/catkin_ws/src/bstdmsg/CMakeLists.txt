cmake_minimum_required(VERSION 2.8.3)
project(bstdmsg)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  std_msgs
  roscpp
)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS python)
find_package(PythonLibs 3.8 REQUIRED) # sets ${PYTHON_INCLUDE_DIRS}


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
catkin_python_setup() # this sets up the path /devel/lib/python2.7/dist-packages/boostpy_test

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES boostpy_test
   LIBRARIES std_msgs_cpp std_msgs_String
#  CATKIN_DEPENDS roscpp rospy
   CATKIN_DEPENDS std_msgs roscpp
#  DEPENDS system_lib
)

catkin_install_python(PROGRAMS scripts/std_msgs_cpp.py
                      DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
)

add_library(std_msgs_String SHARED
  src/std_msgs_String.cpp
)
add_library(std_msgs_cpp src/std_msgs.cpp)
# change output directory, so python can find the module, and set prefix to ""
# to omit the default "lib".
set_target_properties(std_msgs_cpp PROPERTIES
  PREFIX ""
  LIBRARY_OUTPUT_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_PYTHON_DESTINATION}
)

target_link_libraries(std_msgs_cpp
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}

)


install(TARGETS std_msgs_cpp
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_PYTHON_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_PYTHON_DESTINATION}
)


install(TARGETS std_msgs_String
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_PYTHON_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_PYTHON_DESTINATION}
)

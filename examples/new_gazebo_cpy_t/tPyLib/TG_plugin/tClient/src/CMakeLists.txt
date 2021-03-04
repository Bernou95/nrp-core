aux_source_directory(. DIR_SRCS)

find_package(Gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

find_package(PythonLibs 3 REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)
set(CMAKE_SHARED_MODULE_PREFIX "")

add_library(tClient MODULE ${DIR_SRCS})

# Set up the libraries and header search paths for this target
target_link_libraries(tClient ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${GAZEBO_LIBRARIES})
target_include_directories(tClient PRIVATE ${PYTHON_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})

#add_executable(main ${DIR_SRCS})
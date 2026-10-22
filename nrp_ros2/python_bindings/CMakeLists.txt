set(PROJECT_NAME "nrp_ros2_python_bindings")
set(HEADER_DIRECTORY "python_bindings")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
# Same python module as the ROS 1 bindings, so that functions using them don't need to be changed
set(PYTHON_MODULE_NAME "nrp_ros")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")

set(GEN_PYTHON_ROOT ${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_NAME})

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)


# List testing build files
set(TEST_SRC_FILES
        tests/rosmsg_converter.cpp
        )

##########################################
## Macro definitions which perform the python bindings generation

# This macro adds boost python bindings target
macro(add_python_target ROS_PACKAGE)
    set(PY_TARGET "nrp_ros2_${ROS_PACKAGE}")

    add_library(${PY_TARGET} SHARED ${ARGN})

    set_target_properties(${PY_TARGET}
            PROPERTIES
            OUTPUT_NAME ${ROS_PACKAGE}
            LIBRARY_OUTPUT_DIRECTORY ${GEN_PYTHON_ROOT}
            PREFIX ""
            )

    target_include_directories(${PY_TARGET}
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            )

    target_link_libraries(${PY_TARGET}
            ${NRP_GEN_LIB_TARGET}
            ${ROS_PACKAGE}::${ROS_PACKAGE}__rosidl_generator_cpp
            )

    list(APPEND NRP_ROS2_PY_TARGETS ${PY_TARGET})
endmacro()

# Top level macro generates boost python bindings as well as python conversion wrapper
# for all messages in a given ros package `ROS_PACKAGE`
macro(gen_ros_package_python ROS_PACKAGE)
    set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/${ROS_PACKAGE}) #this is for generated cpp files.

    find_package(${ROS_PACKAGE} REQUIRED)

    file(REMOVE_RECURSE ${GEN_DIR})
    execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_pkg_bindings.py
            --package=${ROS_PACKAGE}
            --cpp_target_dir=${GEN_DIR}
            --py_target_dir=${GEN_PYTHON_ROOT}
            --msg_types_file=${CMAKE_BINARY_DIR}/nrp_ros2_msg_types.txt
            --msg_headers_file=${CMAKE_BINARY_DIR}/nrp_ros2_msg_headers.txt
            RESULT_VARIABLE GEN_RESULT
            )
    if(NOT GEN_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to generate python bindings for ROS 2 package ${ROS_PACKAGE}")
    endif()

    file(GLOB ${ROS_PACKAGE}_srcs  ${GEN_DIR}/*.cpp)

    add_python_target(${ROS_PACKAGE}
            ${${ROS_PACKAGE}_srcs}
            )
endmacro()

##########################################
## Create init files in python module containing the generated python bindings
file(WRITE ${GEN_PYTHON_ROOT}/__init__.py "\n")

##########################################
file(WRITE "${CMAKE_BINARY_DIR}/nrp_ros2_msg_types.txt" "")
file(WRITE "${CMAKE_BINARY_DIR}/nrp_ros2_msg_headers.txt" "")

## Generate python bindings for nrp-core msg definitions
gen_ros_package_python(nrp_ros_msgs)
## Generate python bindings for additional external ros packages
foreach(PACK ${NRP_ROS2_MSGS_PACKAGES})
    gen_ros_package_python(${PACK})
endforeach()

#########################################
## Install python modules
install(DIRECTORY ${GEN_PYTHON_ROOT}
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/data")



## Header configuration

configure_file("python_bindings/config/cmake_constants.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)

# Install helper header, used by the generated bindings
install(FILES "python_bindings/ros2_msg_bindings.h"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY})


##########################################
## Tests
if(${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")
    # Create testing executable
    enable_testing()
    add_executable(${TEST_NAME} ${TEST_SRC_FILES})
    target_compile_options(${TEST_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
    target_link_options(${TEST_NAME} PUBLIC ${NRP_COMMON_LD_FLAGS})
    target_link_libraries(${TEST_NAME}
            PUBLIC
            ${NRP_GEN_LIB_TARGET}
            nrp_ros_msgs::nrp_ros_msgs__rosidl_generator_cpp
            GTest::GTest
            GTest::Main)

    # The test imports the installed python bindings, as the ROS 1 test does
    add_dependencies(${TEST_NAME} ${NRP_ROS2_PY_TARGETS})

    # The rclpy conversions need the python package of nrp_ros_msgs
    gtest_discover_tests(${TEST_NAME}
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
            EXTRA_ARGS -VV
            PROPERTIES ENVIRONMENT_MODIFICATION
            "PYTHONPATH=path_list_prepend:${NRP_ROS2_MSGS_INSTALL_DIR}/lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages;LD_LIBRARY_PATH=path_list_prepend:${NRP_ROS2_MSGS_INSTALL_DIR}/lib")

    target_include_directories(${TEST_NAME} BEFORE
            PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

            PRIVATE
            )
endif()




##########################################
## Installation

set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# create cmake version and config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${LIB_VERSION_NAME}.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${LIB_CONFIG_NAME}.cmake"
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
        )

# Install cmake version and config files
install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${LIB_CONFIG_NAME}.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${LIB_VERSION_NAME}.cmake"
        DESTINATION ${INSTALL_CONFIGDIR}
)

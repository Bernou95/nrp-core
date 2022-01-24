set(PROJECT_NAME "NRPDockerSimEngine")
set(HEADER_DIRECTORY "nrp_docker_sim_engine")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(PYTHON_MODULE_NAME "docker_sim")
set(EXECUTABLE_NAME "NRPDockerExecutable")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)
include(ExternalProject)

# List library build files
set(LIB_SRC_FILES
    nrp_docker_sim_engine/config/docker_config.cpp
    nrp_docker_sim_engine/nrp_client/docker_sim_engine.cpp
)

# List of python module build files
set(PYTHON_MODULE_SRC_FILES
)

# List executable build files
set(EXEC_SRC_FILES
)

# List testing build files
set(TEST_SRC_FILES
)


##########################################
## Dependencies

## Python
find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(PythonLibs 3 REQUIRED)

## Boost Python
find_package(Boost REQUIRED)
if(Boost_VERSION VERSION_GREATER_EQUAL 1.71)
    set(BOOST_PYTHON_COMPONENT python)
else()
    set(BOOST_PYTHON_COMPONENT python3)
endif()


##########################################
## Header configuration

##########################################
# NRPPythonJSONEngine
add_library("${LIBRARY_NAME}" SHARED ${LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

set_target_properties(${LIBRARY_NAME} PROPERTIES PREFIX "")

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        NRPJSONEngineProtocol::NRPJSONEngineProtocol

    PRIVATE
)

target_include_directories(${LIBRARY_NAME} BEFORE
    PUBLIC
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

    PRIVATE
)


##########################################
## nrp_core.engines.python_json

##########################################
## NRPPythonExecutable

##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" ${EXEC_SRC_FILES} ${LIB_SRC_FILES} ${PYTHON_MODULE_SRC_FILES} ${HEADER_DIRECTORY})


##########################################
## Tests
if(${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")
    # Create testing env files
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/test_files/test_env.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/test_env.sh" @ONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/test_env_cmake.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/tests/test_env_cmake.h" @ONLY)

    # Create testing executable
    enable_testing()
    add_executable(${TEST_NAME} ${TEST_SRC_FILES})
    target_compile_options(${TEST_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
    target_link_options(${TEST_NAME} PUBLIC ${NRP_COMMON_LD_FLAGS})
    target_link_libraries(${TEST_NAME}
        PUBLIC
        ${NAMESPACE_NAME}::${LIBRARY_NAME}
        GTest::GTest
        GTest::Main)

    gtest_discover_tests(${TEST_NAME}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
        EXTRA_ARGS -VV)
endif()


##########################################
## Installation

set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# Install library files
install(TARGETS
        ${LIBRARY_NAME}
    EXPORT
        ${LIB_EXPORT_NAME}
    LIBRARY DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
    ARCHIVE DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY}
)

# Install export target
install(EXPORT ${LIB_EXPORT_NAME}
    DESTINATION
        ${INSTALL_CONFIGDIR}
    FILE
        "${LIB_EXPORT_NAME}.cmake"
    NAMESPACE
        "${NAMESPACE_NAME}::"
)

# Install headers
install(DIRECTORY "${HEADER_DIRECTORY}" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
)

# Install python module
if(TARGET ${PYTHON_MODULE_NAME})
    install(TARGETS ${PYTHON_MODULE_NAME}
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/engines/${PYTHON_MODULE_NAME}")
endif()



# Install executable files
if(TARGET ${EXECUTABLE_NAME})
    install(TARGETS ${EXECUTABLE_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

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

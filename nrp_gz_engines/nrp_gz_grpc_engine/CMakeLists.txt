set(PROJECT_NAME "NRPGzGrpcEngine")
set(HEADER_DIRECTORY "nrp_gz_grpc_engine")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(SERVER_LIBRARY_NAME "${PROJECT_NAME}Server")
set(SYSTEMS_LIBRARY_NAME "NRPGzGrpcSystems")
set(EXECUTABLE_NAME "${PROJECT_NAME}Executable")
set(EXECUTABLE_ASYNC_NAME "${PROJECT_NAME}Async")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")
set(ENV{NRP_ENGINE_LAUNCHERS} "${LIBRARY_NAME}.so;$ENV{NRP_ENGINE_LAUNCHERS}")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)

# List library build files
set(LIB_SRC_FILES
    nrp_gz_grpc_engine/nrp_client/gz_engine_grpc_nrp_client.cpp
)

# List server library build files
set(SERVER_LIB_SRC_FILES
    nrp_gz_grpc_engine/engine_server/gz_grpc_engine.cpp
    nrp_gz_grpc_engine/engine_server/nrp_world_system.cpp
)

# List gz-sim systems build files
set(SYSTEMS_SRC_FILES
    gz_grpc_systems/nrp_joint_controller_system.cpp
    gz_grpc_systems/nrp_link_controller_system.cpp
    gz_grpc_systems/nrp_model_controller_system.cpp
    gz_grpc_systems/nrp_camera_controller_system.cpp
)

# List executable build files
set(EXEC_SRC_FILES
    nrp_gz_grpc_engine/server_executable/main.cpp
)

# List async executable build files (Event Loop, needs MQTT)
set(EXEC_ASYNC_SRC_FILES
    nrp_gz_grpc_engine/server_executable/main_async.cpp
)

# List testing build files
set(TEST_SRC_FILES
    tests/test_gz_engine.cpp
)


##########################################
## Header configuration

# General Header defines
configure_file("nrp_gz_grpc_engine/config/cmake_constants.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)


##########################################
# NRPGzGrpcEngine
add_library("${LIBRARY_NAME}" SHARED ${LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

set_target_properties(${LIBRARY_NAME} PROPERTIES PREFIX "")

target_include_directories(${LIBRARY_NAME} BEFORE
    PUBLIC
        "$<INSTALL_INTERFACE:include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

    PRIVATE
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        NRPGRPCEngineProtocol::NRPGRPCEngineProtocol

    PRIVATE
)


##########################################
# NRPGzGrpcEngineServer
add_library("${SERVER_LIBRARY_NAME}" SHARED ${SERVER_LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${SERVER_LIBRARY_NAME} ALIAS ${SERVER_LIBRARY_NAME})
target_compile_options(${SERVER_LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

target_include_directories(${SERVER_LIBRARY_NAME} BEFORE
    PUBLIC
        "$<INSTALL_INTERFACE:include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

    PRIVATE
)

target_link_libraries(${SERVER_LIBRARY_NAME}
    PUBLIC
        ${NRP_GEN_LIB_TARGET}
        NRPGRPCEngineProtocol::NRPGRPCEngineProtocol
        NRPProtobuf::ProtoGazebo
        gz-sim8::gz-sim8
        gz-transport13::gz-transport13
        gz-msgs10::gz-msgs10
        gz-math7::gz-math7
        sdformat14::sdformat14

    PRIVATE
)


##########################################
## NRPGzGrpcSystems
add_library(${SYSTEMS_LIBRARY_NAME} SHARED ${SYSTEMS_SRC_FILES})
add_library(${NAMESPACE_NAME}::${SYSTEMS_LIBRARY_NAME} ALIAS ${SYSTEMS_LIBRARY_NAME})
target_compile_options(${SYSTEMS_LIBRARY_NAME} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

target_include_directories(${SYSTEMS_LIBRARY_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(${SYSTEMS_LIBRARY_NAME}
    PUBLIC
        ${NAMESPACE_NAME}::${SERVER_LIBRARY_NAME}

    PRIVATE
        gz-plugin2::register
)


##########################################
## Executable
if(NOT "${EXEC_SRC_FILES}" STREQUAL "")
    add_executable("${EXECUTABLE_NAME}" ${EXEC_SRC_FILES})
    target_link_libraries("${EXECUTABLE_NAME}" ${NAMESPACE_NAME}::${SERVER_LIBRARY_NAME})
endif()

if(NOT "${EXEC_ASYNC_SRC_FILES}" STREQUAL "" AND ENABLE_MQTT)
    add_executable("${EXECUTABLE_ASYNC_NAME}" ${EXEC_ASYNC_SRC_FILES})
    target_link_libraries("${EXECUTABLE_ASYNC_NAME}"
        ${NAMESPACE_NAME}::${SERVER_LIBRARY_NAME}
        ${NAMESPACE_NAME}::${LIBRARY_NAME}
        ${NRP_ELE_TARGET})
endif()


##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" ${EXEC_SRC_FILES} ${EXEC_ASYNC_SRC_FILES} ${LIB_SRC_FILES} ${SERVER_LIB_SRC_FILES} ${SYSTEMS_SRC_FILES} ${HEADER_DIRECTORY})


##########################################
## Tests
if(${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")
    # Create testing env files
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/test_env_cmake.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/tests/test_env_cmake.h" @ONLY)

    # Create testing executable
    enable_testing()
    add_executable(${TEST_NAME} ${TEST_SRC_FILES})
    target_compile_options(${TEST_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
    target_link_options(${TEST_NAME} PUBLIC ${NRP_COMMON_LD_FLAGS})
    target_link_libraries(${TEST_NAME}
        PUBLIC
        ${NAMESPACE_NAME}::${LIBRARY_NAME}
        NRPProtobuf::ProtoGazebo
        GTest::GTest
        GTest::Main)

    # The engine executable and the systems are looked up from the build tree
    add_dependencies(${TEST_NAME} ${EXECUTABLE_NAME} ${SYSTEMS_LIBRARY_NAME})

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
        ${SERVER_LIBRARY_NAME}
    EXPORT
        ${LIB_EXPORT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY}
)

# Install gz-sim systems
install(TARGETS ${SYSTEMS_LIBRARY_NAME}
    LIBRARY DESTINATION ${NRP_GZ_PLUGINS_INSTALL_DIR}
    ARCHIVE DESTINATION ${NRP_GZ_PLUGINS_INSTALL_DIR})

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

# Install executable files
if(TARGET ${EXECUTABLE_NAME})
    install(TARGETS ${EXECUTABLE_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(TARGET ${EXECUTABLE_ASYNC_NAME})
    install(TARGETS ${EXECUTABLE_ASYNC_NAME}
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

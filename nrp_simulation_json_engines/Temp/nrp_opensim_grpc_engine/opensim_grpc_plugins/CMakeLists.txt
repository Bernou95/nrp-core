cmake_minimum_required(VERSION 3.16)
project(NRPOpensimGrpcPlugins VERSION ${NRP_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)


find_package(Pistache REQUIRED)
find_package(nlohmann_json REQUIRED)

if(NOT DEFINED NRP_OPENSIM_COMMUNICATION_PLUGIN)
	set(NRP_OPENSIM_COMMUNICATION_PLUGIN NRPOpensimGrpcCommunicationPlugin)
endif()
include_directories(..)
########################################
## Opensim Communication Plugin
add_library(${NRP_OPENSIM_COMMUNICATION_PLUGIN} SHARED "opensim_communication_controller/nrp_opensim_communication_controller.cpp" "opensim_communication_controller/nrp_opensim_communication_plugin.cpp")
add_library("NRPOpensimGrpcPlugins::${NRP_OPENSIM_COMMUNICATION_PLUGIN}" ALIAS ${NRP_OPENSIM_COMMUNICATION_PLUGIN})
target_compile_options(${NRP_OPENSIM_COMMUNICATION_PLUGIN} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

target_include_directories(${NRP_OPENSIM_COMMUNICATION_PLUGIN}
	PUBLIC
	    $<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		#$<BUILD_INTERFACE:${OPENSIM_INCLUDE_DIRS}>

	PRIVATE
)

target_link_libraries(${NRP_OPENSIM_COMMUNICATION_PLUGIN}
	PUBLIC
	    ${NRP_OPENSIM_LIB_TARGET}

	PRIVATE
		${NRP_GEN_LIB_TARGET}
		NRPGRPCEngineProtocol::NRPGRPCEngineProtocol
)


########################################
## Opensim World Plugin
add_library(NRPOpensimGrpcWorldPlugin SHARED "opensim_world_plugin/nrp_opensim_world_plugin.cpp")
add_library(NRPOpensimGrpcPlugins::NRPOpensimGrpcWorldPlugin ALIAS NRPOpensimGrpcWorldPlugin)
target_compile_options(NRPOpensimGrpcWorldPlugin PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
target_include_directories(NRPOpensimGrpcWorldPlugin
	PUBLIC
	    $<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		#$<BUILD_INTERFACE:${OPENSIM_INCLUDE_DIRS}>

	PRIVATE
	    ${NRP_OPENSIM_LIB_TARGET}

)

target_link_libraries(NRPOpensimGrpcWorldPlugin
	PUBLIC
	    ${NRP_OPENSIM_LIB_TARGET}

	PRIVATE
	    ${NRP_GEN_LIB_TARGET}
		${NRP_OPENSIM_COMMUNICATION_PLUGIN}
		NRPGRPCEngineProtocol::NRPGRPCEngineProtocol
)


########################################
## Opensim Joint Controller Plugin
add_library(NRPOpensimGrpcJointControllerPlugin SHARED "opensim_joint_controller/nrp_opensim_joint_controller.cpp")
add_library(NRPOpensimGrpcPlugins::NRPOpensimGrpcJointControllerPlugin ALIAS NRPOpensimGrpcJointControllerPlugin)
target_compile_options(NRPOpensimGrpcJointControllerPlugin PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

target_include_directories(NRPOpensimGrpcJointControllerPlugin
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		#$<BUILD_INTERFACE:${OPENSIM_INCLUDE_DIRS}>
)

target_link_libraries(NRPOpensimGrpcJointControllerPlugin
	PUBLIC
	    ${NRP_OPENSIM_LIB_TARGET}

	PRIVATE
	    ${NRP_GEN_LIB_TARGET}
		${NRP_OPENSIM_COMMUNICATION_PLUGIN}
		NRPGRPCEngineProtocol::NRPGRPCEngineProtocol
)

##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" "")


########################################
## Configure targets
set(PLUGIN_TARGETS ${NRP_OPENSIM_COMMUNICATION_PLUGIN} NRPOpensimGrpcWorldPlugin
	NRPOpensimGrpcJointControllerPlugin)

set_target_properties(${PLUGIN_TARGETS}
	PROPERTIES PREFIX "")

list(JOIN PLUGIN_TARGETS "," PLUGIN_TARGETS_STRING)
set(NRP_OPENSIM_PLUGINS "${PLUGIN_TARGETS_STRING}"
	CACHE INTERNAL "List of compiled NRP Opensim plugins")


########################################
## Installation
install(TARGETS	${PLUGIN_TARGETS}
	LIBRARY DESTINATION ${NRP_OPENSIM_PLUGINS_INSTALL_DIR}
	ARCHIVE DESTINATION ${NRP_OPENSIM_PLUGINS_INSTALL_DIR})

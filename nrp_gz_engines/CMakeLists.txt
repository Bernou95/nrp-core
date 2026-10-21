cmake_minimum_required(VERSION 3.16)
project(GzEngines VERSION ${NRP_VERSION})

##########################################
## Gazebo Harmonic dependencies

find_package(gz-sim8 REQUIRED)
find_package(gz-transport13 REQUIRED)
find_package(gz-msgs10 REQUIRED)
find_package(gz-math7 REQUIRED)
find_package(gz-plugin2 REQUIRED COMPONENTS register)
find_package(sdformat14 REQUIRED)

set(NRP_GZ_PLUGINS_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/nrp_gz_plugins" CACHE PATH "Install location of gz-sim system plugins")

set(NRP_GZ_PLUGINS_INSTALL_DIR_ABSOLUTE ${NRP_GZ_PLUGINS_INSTALL_DIR})
if(NOT IS_ABSOLUTE ${NRP_GZ_PLUGINS_INSTALL_DIR})
    set(NRP_GZ_PLUGINS_INSTALL_DIR_ABSOLUTE ${CMAKE_INSTALL_PREFIX}/${NRP_GZ_PLUGINS_INSTALL_DIR})
endif()

# add to nrp env vars
list(APPEND NRP_ENV_VARS "GZ_SIM_SYSTEM_PLUGIN_PATH=${NRP_GZ_PLUGINS_INSTALL_DIR_ABSOLUTE}:\${GZ_SIM_SYSTEM_PLUGIN_PATH}")
list(APPEND NRP_ENV_VARS "LD_LIBRARY_PATH=${NRP_GZ_PLUGINS_INSTALL_DIR_ABSOLUTE}:\${LD_LIBRARY_PATH}")
set(NRP_ENV_VARS ${NRP_ENV_VARS} PARENT_SCOPE)

##########################################
## Gazebo Harmonic Engines

add_subdirectory(nrp_gz_grpc_engine)

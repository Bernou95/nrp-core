set(PROJECT_NAME "NRPCorePyClient")
set(HEADER_DIRECTORY "nrp_client")

set(NAMESPACE_NAME "${PROJECT_NAME}")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)

# List library build files
set(PROJECT_SRC_FILES
    nrp_client/__init__.py
    nrp_client/docker_handle.py
    nrp_client/nrp_server_docker_launchers.py
    nrp_client/nrp_server_launchers.py
)

# List library build files
set(PROJECT_CFG_FILES
    setup.py
    pyproject.toml
)

file(COPY ${PROJECT_SRC_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/${HEADER_DIRECTORY}/")
file(COPY ${PROJECT_CFG_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/")

# General Header defines
configure_file("setup.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/setup.cfg" @ONLY)

add_custom_target(nrp-py-client-dev-nodeps ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}
    COMMAND pip install . -U -t "${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_DIR_REL}/" --no-deps
    COMMENT "Installing NRP-Core Python client into NRP-Core installation folder"
    VERBATIM
)

add_custom_target(nrp-py-client
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}
    COMMAND pip install . -U
    COMMENT "Installing NRP-Core Python client into Python site-packages"
    VERBATIM
)

add_custom_target(nrp-py-client-docker
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}
    COMMAND pip install .[docker] -U
    COMMENT "Installing NRP-Core Python client with Docker support into Python site-packages"
    VERBATIM
)

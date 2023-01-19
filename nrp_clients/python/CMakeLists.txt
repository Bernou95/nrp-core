set(PROJECT_NAME "NRPCorePyClient")
set(HEADER_DIRECTORY "nrp_client")

set(NAMESPACE_NAME "${PROJECT_NAME}")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)

# List library build files
set(PROJECT_SRC_FILES
    nrp_client/__init__.py
    nrp_client/nrp_server_docker_launchers.py
    nrp_client/nrp_server_launchers.py
)

# List library build files
set(PROJECT_ROOT_FILES
    setup.py
    pyproject.toml
)

# Generates Python proto code from file PROTO_FILE_NAME. Generated files are appended to var PROTO_PYTHON_FILES_VAR
function(GENERATE_PROTO_PYTHON PROTO_FILE_NAME PROTO_PYTHON_FILES_VAR)
    # List of protobuf proto files
    get_filename_component(hw_proto "${PROTO_FILE_NAME}" ABSOLUTE)
    get_filename_component(hw_proto_name "${hw_proto}" NAME_WE)

    # Generated sources
    set(proto_python "${CMAKE_CURRENT_BINARY_DIR}/proto_gen/${HEADER_DIRECTORY}/pb/${hw_proto_name}_pb2.py")

    # Generate
    add_custom_command(
            OUTPUT "${proto_python}"
            COMMAND "mkdir" ARGS -p "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/${HEADER_DIRECTORY}/pb/"
            COMMAND python3
            ARGS -m grpc_tools.protoc
            --proto_path "${CMAKE_CURRENT_BINARY_DIR}/proto_gen/"
            --python_out "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/"
            "${hw_proto}"
            COMMENT "Generating Python proto file ${proto_python}"
            DEPENDS "${hw_proto}")

    set(${PROTO_PYTHON_FILES_VAR} ${${PROTO_PYTHON_FILES_VAR}} ${proto_python} PARENT_SCOPE)

endfunction()

# Generates Python grpc code from file PROTO_FILE_NAME. Generated files are appended to var PROTO_PYTHON_FILES_VAR
function(GENERATE_GRPC_PYTHON PROTO_FILE_NAME PROTO_PYTHON_FILES_VAR)
    # List of protobuf proto files
    get_filename_component(hw_proto "${PROTO_FILE_NAME}" ABSOLUTE)
    get_filename_component(hw_proto_name "${hw_proto}" NAME_WE)

    # Generated sources
    set(proto_python "${CMAKE_CURRENT_BINARY_DIR}/proto_gen/${HEADER_DIRECTORY}/pb/${hw_proto_name}_pb2_grpc.py")

    # Generate
    add_custom_command(
            OUTPUT "${proto_python}"
            COMMAND "mkdir" ARGS -p "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/${HEADER_DIRECTORY}/pb/"
            COMMAND python3
            ARGS -m grpc_tools.protoc
            --proto_path "${CMAKE_CURRENT_BINARY_DIR}/proto_gen/"
            --grpc_python_out "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/"
            "${hw_proto}"
            COMMENT "Generating Python gRPC file ${proto_python}"
            DEPENDS "${hw_proto}")

    set(${PROTO_PYTHON_FILES_VAR} ${${PROTO_PYTHON_FILES_VAR}} ${proto_python} PARENT_SCOPE)

endfunction()

# Module setuptools configuration file
configure_file("setup.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/setup.cfg" @ONLY)

# Python proto files
set(PROTO_PYTHON_FILES "")
# Copy protofile to the directory tree of the future Python module (for the proper import in *_grpc.py)
file(
    COPY "${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/nrp_server.proto" 
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/proto_gen/${HEADER_DIRECTORY}/pb/"
)
generate_proto_python("${CMAKE_CURRENT_BINARY_DIR}/proto_gen/${HEADER_DIRECTORY}/pb/nrp_server.proto" PROTO_PYTHON_FILES)
generate_grpc_python("${CMAKE_CURRENT_BINARY_DIR}/proto_gen/${HEADER_DIRECTORY}/pb/nrp_server.proto" PROTO_PYTHON_FILES)

# Copy all files to the module directory tree
file(COPY ${PROJECT_SRC_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/${HEADER_DIRECTORY}/")
file(COPY ${PROJECT_ROOT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}/")

# Add Python installation targets
add_custom_target(nrp-py-client-dev-nodeps ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}
    COMMAND pip install . -U -t "${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_DIR_REL}/" --no-deps
    COMMENT "Installing NRP-Core Python client into NRP-Core installation folder"
    DEPENDS ${PROTO_PYTHON_FILES}
    VERBATIM
)

add_custom_target(nrp-py-client
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}
    COMMAND pip install . -U
    COMMENT "Installing NRP-Core Python client into Python site-packages"
    DEPENDS ${PROTO_PYTHON_FILES}
    VERBATIM
)

add_custom_target(nrp-py-client-docker
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIRECTORY}
    COMMAND pip install .[docker] -U
    COMMENT "Installing NRP-Core Python client with Docker support into Python site-packages"
    DEPENDS ${PROTO_PYTHON_FILES}
    VERBATIM
)

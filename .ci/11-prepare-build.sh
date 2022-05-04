#!/usr/bin/env bash

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

# Create a build directory in the root directory

rm -rf "$repo_root"/build
mkdir -p "$repo_root"/build
cd "$repo_root"/build || exit 1;

if [ -z "$NRP_INSTALL_DIR" ]; then
    echo "NRP_INSTALL_DIR is unset"
    exit 1
fi

# Set up the environment

source "$HOME"/.bashrc

# Check if NEST_INSTALL_DIR is set and the external nest-simulator can be used

[[ -z "$NEST_INSTALL_DIR" ]] && NEST_INSTALL_OPTION="" || NEST_INSTALL_OPTION="-DNEST_INSTALL_DIR=${NEST_INSTALL_DIR}"

# Check if NEST_INSTALL_DIR is set and the external nest-simulator can be used

[[ -z "$CMAKE_CACHE_FILE" ]] && CMAKE_CACHE_FILE="${repo_root}/.ci/cmake_cache/vanilla.cache"

# Run cmake

cmake .. -DCMAKE_INSTALL_PREFIX="$NRP_INSTALL_DIR" "${NEST_INSTALL_OPTION}" -C "$CMAKE_CACHE_FILE"

# EOF

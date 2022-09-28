#!/bin/bash
set -e

GREEN='\033[0;32m'
NC='\033[0m' # No Color


export NRP_INSTALL_DIR="/home/${USER}/.local/nrp"
export NRP_DEPS_INSTALL_DIR="/home/${USER}/.local/nrp_deps"

# Start of dependencies installation
# Pistache REST Server
sudo add-apt-repository ppa:pistache+team/unstable

# Gazebo repository
sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
wget https://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -

sudo apt update
sudo apt install git cmake libpistache-dev libboost-python-dev libboost-filesystem-dev libboost-numpy-dev libcurl4-openssl-dev nlohmann-json3-dev libzip-dev cython3 python3-numpy libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev doxygen libgsl-dev libopencv-dev python3-opencv python3-pil python3-pip libgmock-dev

# required by gazebo engine
sudo apt install libgazebo11-dev gazebo11 gazebo11-plugin-base

# Remove flask if it was installed to ensure it is installed from pip
sudo apt remove python3-flask python3-flask-cors
# required by Python engine
# If you are planning to use The Virtual Brain framework, you will most likely have to use flask version 1.1.4.
# By installing flask version 1.1.4 markupsafe library (included with flask) has to be downgraded to version 2.0.1 to run properly with gunicorn
# You can install that version with
# pip install flask==1.1.4 gunicorn markupsafe==2.0.1
pip install flask gunicorn

# required by nest-server (which is built and installed along with nrp-core)
sudo apt install python3-restrictedpython uwsgi-core uwsgi-plugin-python3
pip install flask_cors mpi4py docopt

# required by nrp-server, which uses gRPC python bindings
pip install grpcio-tools pytest psutil docker

# Required for using docker with ssh
pip install paramiko

# ROS

sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
sudo apt update
sudo apt install ros-noetic-ros-base


#Tell nrp-core where your catkin workspace is located: export a variable CATKIN_WS pointing to an existing catkin workspace root folder. If the variable does not exist, a new catkin workspace will be created at `${HOME}/catkin_ws`.
export CATKIN_WS=${HOME}/catkin_ws


# MQTT Paho library, required by datatransfer engine for streaming data over network
# More information on the project web site https://github.com/eclipse/paho.mqtt.cpp
# If you do not want to add network data streaming feature, you can skip this step.
# MQTT Paho C library
pushd ${HOME}

git clone https://github.com/eclipse/paho.mqtt.c.git
pushd paho.mqtt.c >/dev/null
git checkout v1.3.8
cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON
sudo cmake --build build/ --target install
sudo ldconfig
popd >/dev/null


# MQTT Paho CPP
git clone https://github.com/eclipse/paho.mqtt.cpp
pushd paho.mqtt.cpp >/dev/null
git checkout v1.2.0
cmake -Bbuild -H. -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DCMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}" -DCMAKE_PREFIX_PATH="${NRP_DEPS_INSTALL_DIR}"
cmake --build build/ --target install
sudo ldconfig
popd >/dev/null

popd >/dev/null
# End of dependencies installation

# Start of installation

mkdir build
cd build
# See the section "Common NRP-core CMake options" in the documentation for the additional ways to configure the project with CMake
. /opt/ros/noetic/setup.bash
cmake .. -DCMAKE_INSTALL_PREFIX="${NRP_INSTALL_DIR}" -DNRP_DEP_CMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}"
mkdir -p "${NRP_INSTALL_DIR}"
# the installation process might take some time, as it downloads and compiles Nest as well.
# If you haven't installed MQTT libraries, add ENABLE_MQTT=OFF definition to cmake (-DENABLE_MQTT=OFF).
make -j8
make install
# just in case of wanting to build the documentation. Documentation can then be found in a new doxygen folder
make nrp_doxygen

# End of installation

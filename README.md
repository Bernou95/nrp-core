This README file contains information on how to get nrp-core installed in your system. Information on how to get started with nrp-core, architecture details, and much more can be found at the nrp-core [online documentation](https://hbpneurorobotics.bitbucket.io/)

**WARNING:** nrp-core has only been tested on Ubuntu 20.04 at the moment and this OS and version are assumed in the instructions below. Installation in other environments might be possible but has not been tested yet. For an installation on Ubuntu 24.04, see the section **Ubuntu 24.04 installation** below.

 * Before starting the installation, define, please, the nrp-core installation directory:
 
 ```
export NRP_INSTALL_DIR="/home/${USER}/.local/nrp"
export NRP_DEPS_INSTALL_DIR="/home/${USER}/.local/nrp_deps"
 ```

## Ubuntu 24.04 installation

nrp-core has also been built and tested on Ubuntu 24.04 (noble). The steps below replace the Ubuntu 20.04 sections **Dependency Installation** and **Installation**.

Current status on Ubuntu 24.04:

 - Gazebo Classic (gazebo11) and ROS Noetic aren't available on Ubuntu 24.04. Gazebo Classic is disabled, and Gazebo Harmonic engines are in progress.
 - ROS 1 support was removed. ROS 2 Jazzy is supported instead (`-DENABLE_ROS2=ON`, `ros-jazzy-ros-base`). `/opt/ros/jazzy/setup.bash` has to be sourced before cmake and when running nrp-core.
 - NEST, MQTT and SpiNNaker are disabled for now. EDLUT is enabled with CUDA support (tested on an RTX 3060, compute capability 8.6).
 - 92/93 tests pass. `EventLoop.EVENT_LOOP` still fails.

Versions used:

| Component | Version |
|---|---|
| GCC | 13.3 |
| CMake | 3.28.3 |
| Python | 3.12.3 |
| Boost | 1.83 |
| LLVM / libclang | 18 |
| gRPC (C++, `libgrpc++-dev`) | 1.51.1 |
| protobuf (C++, `libprotobuf-dev`) | 3.21.12 |
| nlohmann-json | 3.11.3 |
| pistache (`libpistache-dev`) | 0.0.5 |
| json-schema-validator (built by nrp-core) | 2.3.0 |
| CUDA toolkit | 13.2 (NVIDIA driver 595.58.03) |
| EDLUT (fetched by nrp-core from github.com/Bernou95/edlut3) | master @ 0c5b35f |

```bash
# Start of Ubuntu 24.04 installation
export NRP_INSTALL_DIR="/home/${USER}/.local/nrp"
export NRP_DEPS_INSTALL_DIR="/home/${USER}/.local/nrp_deps"

# 1- System dependencies (all from the noble repositories, no PPAs needed)
sudo apt update
sudo apt install cmake build-essential pkg-config git libpistache-dev libboost-python-dev libboost-filesystem-dev libboost-numpy-dev libcurl4-openssl-dev nlohmann-json3-dev libzip-dev cython3 python3-numpy libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev doxygen libgsl-dev libopencv-dev python3-opencv python3-pil python3-pip python3-venv python3-dev libgmock-dev libgtest-dev libclang-dev libomp-dev llvm-dev python3-restrictedpython uwsgi-core uwsgi-plugin-python3

# 2- Python virtual environment
# Ubuntu 24.04 doesn't allow "pip install" into the system Python (PEP 668), so a venv is required.
# --system-site-packages is needed so that the apt packages python3-numpy, python3-opencv, python3-pil, etc. stay visible.
# The venv has to be active when building nrp-core and when running it or its tests.
python3 -m venv --system-site-packages ~/.venvs/nrp
. ~/.venvs/nrp/bin/activate

# 3- Python packages
pip install flask gunicorn paho-mqtt flask_cors docopt pytest psutil docker paramiko python-on-whales pyyaml
# grpcio-tools generates the Python protobuf modules during the build. It must match the protobuf Python runtime,
# and protobuf >= 5 removed MessageFactory.GetMessages (used by nrp_client). Pin both:
pip install "grpcio==1.62.*" "grpcio-tools==1.62.*" "protobuf<5"
# Tested with: grpcio 1.62.3, grpcio-tools 1.62.3, protobuf 4.25.9, flask 3.1.3, gunicorn 26.2.0, pytest 9.1.1
# If these packages are changed after building, do a clean rebuild so that the Python protobuf modules are regenerated.

# 4- CUDA toolkit (for EDLUT with GPU support)
# Unlike the Ubuntu 20.04 instructions, do NOT purge the existing NVIDIA packages and do NOT install the "cuda" meta-package,
# which also installs a driver. With a working NVIDIA driver already installed, only the toolkit is needed:
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt update
sudo apt install cuda-toolkit-13-2
echo 'export PATH=/usr/local/cuda/bin${PATH:+:${PATH}}' >> ~/.bashrc
# Inside a chroot or container (which uses the host's kernel driver) the NVIDIA user-space libraries must match the host driver
# version exactly. E.g. for driver 595.58.03:
#   V=595.58.03-1ubuntu1
#   sudo apt install libnvidia-compute=$V libnvidia-cfg1=$V libnvidia-gpucomp=$V nvidia-persistenced=$V
#   sudo apt-mark hold libnvidia-compute libnvidia-cfg1 libnvidia-gpucomp nvidia-persistenced

# 5- Clone (including the nrp-core-msgs submodule)
git clone https://bitbucket.org/hbpneurorobotics/nrp-core.git
cd nrp-core
# The GitHub URL in .gitmodules may ask for credentials. In that case, redirect the submodule to Bitbucket
# (same commit) with a local configuration only:
git config submodule.nrp-core-msgs.url https://bitbucket.org/hbpneurorobotics/nrp-core-msgs.git
git submodule update --init

# 6- Build and install
mkdir build && cd build
export LD_LIBRARY_PATH=${NRP_DEPS_INSTALL_DIR}/lib:${LD_LIBRARY_PATH}
cmake .. -DCMAKE_INSTALL_PREFIX="${NRP_INSTALL_DIR}" -DNRP_DEP_CMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}" \
    -DENABLE_ROS=OFF -DENABLE_MQTT=OFF -DENABLE_SPINNAKER=OFF -DENABLE_GAZEBO=OFF \
    -DENABLE_NEST=OFF -DENABLE_EDLUT=ON -DEDLUT_WITH_CUDA=ON -DCOVERAGE=OFF
# EDLUT is downloaded and built as part of nrp-core. Its optional Python bindings (pyedlut) fail to install with
# "InvalidVersion: ''" during "make install". This doesn't affect nrp-core, which only uses libedlut.so.
make -j$(nproc)
make install

# 7- Environment (optionally add these lines at the end of ~/.bashrc; the venv still has to be activated)
export NRP_INSTALL_DIR=$HOME/.local/nrp
export NRP_DEPS_INSTALL_DIR=$HOME/.local/nrp_deps
export LD_LIBRARY_PATH=$NRP_DEPS_INSTALL_DIR/lib:$LD_LIBRARY_PATH
[ -f $NRP_INSTALL_DIR/bin/.nrp_env ] && source $NRP_INSTALL_DIR/bin/.nrp_env

# 8- Tests (run them serially: running them in parallel with -j causes port collisions)
ctest --output-on-failure

# End of Ubuntu 24.04 installation
```

## Dependency Installation

```bash
# Start of dependencies installation
# 1 - Pistache REST Server
sudo add-apt-repository ppa:pistache+team/unstable
    
# 2- Gazebo Install
sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
wget https://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
    
sudo apt update
sudo apt install git cmake libpistache-dev libboost-python-dev libboost-filesystem-dev libboost-numpy-dev libcurl4-openssl-dev nlohmann-json3-dev libzip-dev cython3 python3-numpy libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev doxygen libgsl-dev libopencv-dev python3-opencv python3-pil python3-pip libgmock-dev libclang-dev libomp-dev

# required by gazebo engine
sudo apt install libgazebo11-dev gazebo11 gazebo11-plugin-base

# 3- Install required python packages   
# Remove flask if it was installed to ensure it is installed from pip
sudo apt remove python3-flask python3-flask-cors
# required by Python engine
# If you are planning to use The Virtual Brain framework, you will most likely have to use flask version 1.1.4.
# By installing flask version 1.1.4 markupsafe library (included with flask) has to be downgraded to version 2.0.1 to run properly with gunicorn
# You can install that version with 
# pip install flask==1.1.4 gunicorn markupsafe==2.0.1
pip install flask gunicorn paho-mqtt

# required by nest-server (which is built and installed along with nrp-core)
sudo apt install python3-restrictedpython uwsgi-core uwsgi-plugin-python3 
pip install flask_cors mpi4py docopt

# required by nrp-server, which uses gRPC python bindings
pip install "grpcio-tools>=1.49.1" pytest psutil docker

# Required for using docker with ssh
pip install paramiko

# The Python packages 'python_on_whales' and 'pyyaml' are optionally required to invoke nrp-core remotely with the Docker Compose (see guides/remote_docker_compose.dox for details).
pip install python-on-whales pyyaml

   
# 4- Installing ROS

# Install ROS: follow the installation instructions: http://wiki.ros.org/noetic/Installation/Ubuntu. To enable ros support in nrp on `ros-noetic-ros-base` is required.

# 5- Setting CATKIN workspace 
# If there is an existing catkin workspace in your environment and you would like nrp-core to use it, export the variable CATKIN_WS pointing to it:
# E.g. export CATKIN_WS=<path to your catkin workspace>
# Otherwise nrp-core will create and compile a new catkin workspace at: ${HOME}/catkin_ws

# 6- Install SpiNNaker
# Follow the instructions at: https://spinnakermanchester.github.io/development/gitinstall.html.
# Ensure that if using a virtualenv, this is active when running any SpiNNaker scripts.

# 8- Installing Paho MQTT C and CPP 
# MQTT Paho library, required by datatransfer engine for streaming data over network
# More information on the project web site https://github.com/eclipse/paho.mqtt.cpp
# If you do not want to add network data streaming feature, you can skip this step.

# MQTT Paho C library
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
git checkout v1.3.8
cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON -DCMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}"
cmake --build build/ --target install
sudo ldconfig && cd ..

# MQTT Paho CPP
git clone https://github.com/eclipse/paho.mqtt.cpp.git
cd paho.mqtt.cpp
git checkout v1.2.0
cmake -Bbuild -H. -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DCMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}" -DCMAKE_PREFIX_PATH="${NRP_DEPS_INSTALL_DIR}"
cmake --build build/ --target install
sudo ldconfig && cd ..

# CUDA Support
# The EDLUT simulator supports running on CUDA GPUs. This option can be enabled if EDLUT_WITH_CUDA cmake option is set to ON while configuring nrp-core. 
# It is highly recommended to install a CUDA version >=11.0 due to compatibility version with GCC9 (default compiler for Ubuntu 20.04)
# In order to ensure that you can follow these steps which install CUDA 12.0:
sudo apt-get --purge -y remove 'cuda*' 
sudo apt-get --purge -y remove 'nvidia*'
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt update
sudo apt install cuda
echo 'export PATH=/usr/local/cuda/bin${PATH:+:${PATH}}' >> ~/.bashrc

# End of dependencies installation
```

## Installation

```bash
# Start of installation
git clone https://bitbucket.org/hbpneurorobotics/nrp-core.git
cd nrp-core
mkdir build
cd build

export LD_LIBRARY_PATH=${NRP_DEPS_INSTALL_DIR}/lib:${LD_LIBRARY_PATH}
# if you have ROS installed (Step 4 in dependencies installation), you need to source its setup.bash file before cmake. If you don't need ROS (and did not install it) skip the next line. 
. /opt/ros/noetic/setup.bash 
# make sure that NRP_INSTALL_DIR is set properly as mentioned at the beginning of tutorial
# See the section "Common NRP-core CMake options" in the documentation for the additional ways to configure the project with CMake
cmake .. -DCMAKE_INSTALL_PREFIX="${NRP_INSTALL_DIR}" -DNRP_DEP_CMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}"
mkdir -p "${NRP_INSTALL_DIR}"
# the installation process might take some time, as it downloads and compiles Nest as well.
# If you haven't installed MQTT libraries (Step 8 in dependencies installation), add ENABLE_MQTT=OFF definition to cmake (-DENABLE_MQTT=OFF).
make
make install
# just in case of wanting to build the documentation. Documentation can then be found in a new doxygen folder
make nrp_doxygen

# End of installation
```

 * Start the simulation:
```bash
    # Some variables should be set before running the simulation 
    # make sure that you have initialized $NRP_INSTALL_DIR before next command !
    source  ${NRP_INSTALL_DIR}/bin/.nrp_env 
    
    . /usr/share/gazebo-11/setup.sh
    . /opt/ros/noetic/setup.bash

    # If ROS was installed, setup your catkin workspace. 
    # If you let nrp-core create one for you (Step 5- Setting CATKIN workspace in dependencies installation) the path to it will be: ${HOME}/catkin_ws
    . <path to your catkin workspace>/devel/setup.bash # Ex: . ${CATKIN_WS}/devel/setup.bash 
     
    NRPCoreSim -c <SIMULATION_CONFIG_FILE>
```

## Basic Information

 - The project is divided into multiple libraries, separated by folders:
     - nrp_general_library: Main Library. Contains classes and methods to interface with Python, Engines, and Transceiver-Functions
     - nrp_engine_protocols: Engine interfaces implementing server/client communication for different communication protocols
     - nrp_nest_engines: Nest Engine
     - nrp_gazebo_engines: Gazebo Engine
     - nrp_python_json_engine: Python JSON Engine
     - nrp_simulation: Contains the FTILoop and -Manager. Creates the NRPCoreSim executable
 - Each of these folders also contains a 'tests' folder with basic integration testing capabilities. To run the tests, look for generated executables inside the build folder. Before running the tests, setup the environment as described above in **Running an experiment**
 - All libraries generate a python module. This can be used to interface with the datapacks from the TFs. After installation, they will be located inside `${NRP_INSTALL_DIR}/lib/python3.8/site-packages`

## Examples

 - Examples are located in the examples subfolder:
     - To run them, first set the environment as described in **Running an experiment**. Then:

            cd examples/<EXAMPLE_NAME>
            NRPCoreSim -c <SIMULATION_CONFIG>
            
     - If gazebo is running in the experiment, you can use `gzclient` to visualize the gazebo simulation

## Acknowledgments

This repository is part of the Neurorobotics Platform software
Copyright (C) Human Brain Project
https://neurorobotics.ai

The Human Brain Project is a European Commission funded project
in the frame of the [Horizon2020 FET Flagship plan](http://ec.europa.eu/programmes/horizon2020/en/h2020-section/fet-flagships).

This work has received funding from the European Union’s Horizon 2020 Framework Programme for Research and Innovation under the Specific Grant Agreement No. 720270 (Human Brain Project SGA1), and the Specific Grant Agreement No. 785907 (Human Brain Project SGA2), and under the Specific Grant Agreement No. 945539 (Human Brain Project SGA3).

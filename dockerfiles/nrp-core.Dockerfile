# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE} AS nrp-core

# CMake configuration
ARG CMAKE_CACHE_FILE
ENV CMAKE_CACHE_FILE ${CMAKE_CACHE_FILE}

# Install dependencies for testing

RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.tests.txt  | tr "\n" " ")

# Pistache REST Server

RUN sudo add-apt-repository ppa:pistache+team/unstable

# ROS

RUN sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
RUN sudo sh -c 'curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add -'

# Install CLE dependencies

RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.cle.txt  | tr "\n" " ")

# If this image will be used for TVB integration, then flask==1.1.4 is needed and after markupsafe (included in flask) has to be downgraded to 2.0.1
RUN pip install grpcio-tools pytest psutil flask gunicorn flask_cors mpi4py docopt

# Install Documentation dependencies

RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.docs.txt  | tr "\n" " ")

# Install MQTT (to NRP_INSTALL_DIR)
RUN git clone https://github.com/eclipse/paho.mqtt.c.git \
    && cd paho.mqtt.c \
    && git checkout v1.3.8 \
    && cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON -DCMAKE_INSTALL_PREFIX="${NRP_INSTALL_DIR}"\
    && cmake --build build/ --target install \
    && sudo ldconfig && cd .. && rm -rf paho.mqtt.c

RUN git clone https://github.com/eclipse/paho.mqtt.cpp \
    && cd paho.mqtt.cpp \
    && git checkout v1.2.0 \
    && cmake -Bbuild -H. -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DCMAKE_INSTALL_PREFIX="${NRP_INSTALL_DIR}" -DCMAKE_PREFIX_PATH="${NRP_INSTALL_DIR}"\
    && cmake --build build/ --target install \
    && sudo ldconfig && cd .. && rm -rf paho.mqtt.cpp


# Configure and install NRP in a intermediate sub-image

FROM nrp-core AS nrp-core-builder
RUN mkdir -p ${HOME}/nrp-core-src
COPY --chown=${NRP_USER}:${NRP_GROUP} . ${HOME}/nrp-core-src/
RUN cd ${HOME}/nrp-core-src && ls -l && bash .ci/11-prepare-build.sh && bash .ci/20-build.sh


# Copy the installed nrp to the main image (the intermediate container with code will be unseen for production)
FROM nrp-core
COPY --from=nrp-core-builder ${NRP_INSTALL_DIR} ${NRP_INSTALL_DIR}

# EOF

# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}


# Install dependencies for testing

RUN apt-get update && apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.tests.txt  | tr "\n" " ")

# Pistache REST Server

RUN add-apt-repository ppa:pistache+team/unstable

# ROS

RUN sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
RUN curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add -

# Install CLE dependencies

RUN apt-get update && apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.cle.txt  | tr "\n" " ")

# If this image will be used for TVB integration, then flask==1.1.4 is needed and after markupsafe (included in flask) has to be downgraded to 2.0.1
RUN pip install grpcio-tools pytest flask gunicorn flask_cors psutil

# Install Documentation dependencies

RUN apt-get update && apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.docs.txt  | tr "\n" " ")

# Switch to NRP user

USER ${NRP_USER}
ENV USER ${NRP_USER}
WORKDIR ${HOME}

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

# EOF

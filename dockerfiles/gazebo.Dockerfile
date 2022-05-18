# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}


# Add gazebo repository

RUN sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
RUN sudo sh -c 'wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add -'

# Install gazebo libraries and dependencies

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.gazebo.txt ${HOME}/.dependencies/apt/requirements.gazebo.txt
RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.gazebo.txt  | tr "\n" " ")

# Install Gazebo Models. TODO/WARNING: extra building time and container size!!!
RUN mkdir ${HOME}/nrp \
    && cd ${HOME}/nrp \
    && git clone https://@bitbucket.org/hbpneurorobotics/models.git \
    && git clone https://@bitbucket.org/hbpneurorobotics/gzweb.git \
    && export HBP=/home/${USER}/.local/nrp \
    && mkdir -p ${HBP}/gzweb/http/client/assets \
    && mkdir -p ${HOME}/.gazebo/models \
    && cd models \
    && ./create-symlinks.sh

WORKDIR ${HOME}

# EOF

# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}



# Install gazebo libraries and dependencies

RUN curl -sSL http://get.gazebosim.org | sh

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

# Setup gazebo
WORKDIR ${HOME}
RUN echo '. /usr/share/gazebo-11/setup.sh' >> ${HOME}/.bashrc

# Define entrypoint
COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/xvfb-run-gazebo-runcmd.bash /usr/xvfb-run-gazebo-runcmd.bash
RUN chmod +x /usr/xvfb-run-gazebo-runcmd.bash

# EOF

# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

# Install dependecies

RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.tvb.txt  | tr "\n" " ")

# numba (dependency of tvb) requires numpy < 1.22

RUN pip install numpy==1.21

# Install TVB data

WORKDIR ${HOME}
RUN git clone https://github.com/the-virtual-brain/tvb-data.git
WORKDIR ${HOME}/tvb-data
RUN sudo python3 setup.py develop

# Install TVB root

WORKDIR ${HOME}
RUN git clone https://github.com/the-virtual-brain/tvb-root.git
WORKDIR ${HOME}/tvb-root/tvb_build
RUN sh install_full_tvb.sh

# EOF

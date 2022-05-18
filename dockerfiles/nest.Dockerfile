# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}


# Install nest dependencies

RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.nest.txt  | tr "\n" " ")

# Switch to NRP user

USER ${NRP_USER}
ENV USER ${NRP_USER}
WORKDIR ${HOME}

# Install nest-simulator (to NRP_INSTALL_DIR)
RUN git clone https://github.com/nest/nest-simulator.git \
    && cd nest-simulator \
    && git checkout v3.1 \
    && mkdir build && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX:PATH=${NRP_INSTALL_DIR} -Dwith-mpi=ON -Dwith-python=ON .. \
    && make -j4 && make install \
    && cd .. && rm -rf nest-simulator
ENV NEST_INSTALL_DIR ${NRP_INSTALL_DIR}
ENV PYTHONPATH=$NEST_INSTALL_DIR/lib/python3.8/site-packages:$PYTHONPATH

WORKDIR ${HOME}

# EOF

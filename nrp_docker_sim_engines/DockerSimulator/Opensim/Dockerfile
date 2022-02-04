FROM ubuntu:20.04

RUN apt-get update && apt-get install -y
RUN apt-get upgrade -y
RUN apt-get install -y python3
RUN apt-get install -y apt-utils
RUN apt-get install -y dialog
RUN apt-get install -y tzdata
RUN apt-get install -y git wget pkg-config
RUN apt-get install -y build-essential

# For ipopt
RUN apt-get install -y libblas-dev libatlas-base-dev
RUN apt-get install -y gcc g++ gfortran patch libmetis-dev
RUN apt-get install -y coinor-libipopt-dev
# For adolc
RUN apt-get install -y libtool libtool-bin
RUN apt-get install -y autoconf
RUN apt-get install -y libadolc-dev

RUN apt-get update 
RUN apt-get -y install  cmake cmake-curses-gui freeglut3-dev
RUN apt-get -y install  libxi-dev libxmu-dev liblapack-dev swig python-dev \
                           openjdk-8-jdk
ENV JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64

# Install PATH
ARG OPENSIM_PATH=/root/Documents/OpenSim
RUN mkdir -p "$OPENSIM_PATH"
WORKDIR $OPENSIM_PATH

# Build Opensim dependencie
RUN mkdir Document
WORKDIR Document
RUN apt-get update
RUN apt-get install -y libpcre2-dev
RUN apt-get install -y libpcre3 libpcre3-dev
RUN apt-get install -y bison byacc
RUN git clone https://github.com/swig/swig

WORKDIR swig
RUN chmod a+x autogen.sh
RUN /bin/bash -c "source autogen.sh"
RUN /bin/bash -c "./configure"
RUN make
RUN make install
RUN cp swig /usr/bin/swig4.0

WORKDIR $OPENSIM_PATH
RUN git clone https://github.com/opensim-org/opensim-core.git
RUN git clone https://github.com/casadi/casadi.git
RUN git clone https://github.com/simbody/simbody
RUN cp -r casadi opensim-core/dependencies/
RUN cp -r simbody opensim-core/dependencies/


RUN mkdir opensim_dependencies_build
WORKDIR opensim_dependencies_build
RUN cmake ../opensim-core/dependencies/ \
      -DCMAKE_INSTALL_PREFIX='../opensim_dependencies_install' \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo
RUN make -j4

# Environments for opensim
RUN alias python=python3
RUN apt-get install -y python3-pip
RUN pip3 install numpy    

WORKDIR $OPENSIM_PATH
ARG OPENSIM_INSTALL_PATH=/Opensim/OpenSim
RUN mkdir -p $OPENSIM_INSTALL_PATH
RUN mkdir opensim_build
WORKDIR opensim_build
ENV JAVA_TOOL_OPTIONS=-Dfile.encoding=UTF8
RUN cmake ../opensim-core \
      -DCMAKE_INSTALL_PREFIX=$OPENSIM_INSTALL_PATH \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DOPENSIM_DEPENDENCIES_DIR="../opensim_dependencies_install" \
      -DBUILD_PYTHON_WRAPPING=ON \
      -DBUILD_JAVA_WRAPPING=ON \
      -DWITH_BTK=ON
RUN make -j4
RUN make -j4 install

WORKDIR $OPENSIM_PATH
WORKDIR $OPENSIM_INSTALL_PATH/lib/python3.8/site-packages
RUN python3 setup.py install

RUN pip3 install Flask
RUN pip3 install flask-socketio

WORKDIR /root
ADD simulator /root/simulator

RUN /bin/bash -c "echo 'export LD_LIBRARY_PATH='$OPENSIM_INSTALL_PATH'/lib:\$LD_LIBRARY_PATH' >> $HOME/.bashrc"
RUN /bin/bash -c "echo 'export PYTHONPATH='$OPENSIM_INSTALL_PATH'/lib/python3.8/site-packages:\$PYTHONPATH' >> $HOME/.bashrc"
RUN /bin/bash -c "source $HOME/.bashrc"

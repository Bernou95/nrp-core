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

RUN apt-get --yes install  cmake cmake-curses-gui \
                           freeglut3-dev libxi-dev libxmu-dev \
                           liblapack-dev swig python-dev \
                           openjdk-8-jdk
ENV JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64

# Install PATH
ARG OPENSIM_PATH=/root/Documents/OpenSim
RUN mkdir -p "$OPENSIM_PATH"
WORKDIR $OPENSIM_PATH

# Build Opensim dependencie
RUN git clone https://github.com/opensim-org/opensim-core.git
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
RUN mkdir opensim_build
WORKDIR opensim_build
ENV JAVA_TOOL_OPTIONS=-Dfile.encoding=UTF8
RUN cmake ../opensim-core \
      -DCMAKE_INSTALL_PREFIX="../opensim_install" \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DOPENSIM_DEPENDENCIES_DIR="../opensim_dependencies_install" \
      -DBUILD_PYTHON_WRAPPING=ON \
      -DBUILD_JAVA_WRAPPING=ON \
      -DWITH_BTK=ON
RUN make -j4
RUN make -j4 install

WORKDIR $OPENSIM_PATH
WORKDIR opensim_install/lib/python3.8/site-packages
RUN python3 setup.py install

WORKDIR /root
RUN /bin/bash -c "echo 'export LD_LIBRARY_PATH='$OPENSIM_PATH'/opensim_install/lib:\$LD_LIBRARY_PATH' >> $HOME/.bashrc"
RUN /bin/bash -c "echo 'export PYTHONPATH='$OPENSIM_PATH'/opensim_install/lib/python3.8/site-packages:\$PYTHONPATH' >> $HOME/.bashrc"
RUN /bin/bash -c "source $HOME/.bashrc"


# ---------------------------------------------
# For NRP
# ADD nrp-opensim $HOME
RUN git clone -b new-opensim https://bitbucket.org/hbpneurorobotics/nrp-core.git

RUN apt-get update && apt-get install -y
# Install dependencies
RUN apt-get install -y software-properties-common

RUN add-apt-repository ppa:pistache+team/unstable
RUN sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
RUN wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add -
    
RUN apt update
RUN apt install -y git cmake libpistache-dev g++-10 libboost-python-dev libboost-filesystem-dev libboost-numpy-dev libcurl4-openssl-dev nlohmann-json3-dev libzip-dev cython3 python3-numpy libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev doxygen libgsl-dev libopencv-dev python3-opencv python3-pil
 
RUN apt install -y libgazebo11-dev gazebo11 gazebo11-plugin-base
RUN apt install -y python3-flask python3-flask-cors python3-restrictedpython uwsgi-core uwsgi-plugin-python3

RUN sed -i "s/typename std::allocator<void>::const_pointer/const void*/g" /usr/include/OGRE/OgreMemorySTLAllocator.h


WORKDIR /root/nrp-core
RUN /bin/bash -c "source build.sh"

ARG NRP_PATH=/root/.local/nrp
RUN /bin/bash -c "echo 'export PYTHONPATH='$NRP_PATH'/lib/python3.8/site-packages:\$PYTHONPATH' >> $HOME/.bashrc"
RUN /bin/bash -c "echo 'export LD_LIBRARY_PATH='$NRP_PATH'/lib:\$LD_LIBRARY_PATH' >> $HOME/.bashrc"
RUN /bin/bash -c "echo 'export PATH='$NRP_PATH'/bin:\$PATH' >> $HOME/.bashrc"
RUN /bin/bash -c ". /usr/share/gazebo-11/setup.sh"

WORKDIR /root
ADD runTestOpensim.sh $HOME
RUN /bin/bash -c "chmod a+x runTestOpensim.sh"


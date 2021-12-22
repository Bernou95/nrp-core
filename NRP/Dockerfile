FROM ubuntu:20.04

RUN apt-get update && apt-get install -y
RUN apt-get upgrade -y
RUN apt-get install -y python3
RUN apt-get install -y apt-utils
RUN apt-get install -y dialog
RUN apt-get install -y tzdata
RUN apt-get install -y git wget pkg-config
RUN apt-get install -y build-essential
RUN apt-get update && apt-get install -y lsb-release && apt-get clean all
RUN apt-get install -y python3-pip   
# ---------------------------------------------
# For ROS
RUN /bin/bash -c "echo 'deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main' > /etc/apt/sources.list.d/ros-latest.list"
RUN apt install -y curl # if you haven't already installed curl
RUN curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add -
RUN apt update
RUN apt install -y ros-noetic-ros-base
RUN /bin/bash -c  "echo 'source /opt/ros/noetic/setup.bash' >> ~/.bashrc"
RUN apt install -y python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
RUN apt install -y python3-rosdep
# ---------------------------------------------
# For NRP
RUN apt-get update && apt-get install -y
# Install dependencies
RUN apt-get install -y software-properties-common
RUN pip3 install grpcio-tools  
RUN add-apt-repository ppa:pistache+team/unstable
RUN sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
RUN wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add -

RUN apt update
RUN apt install -y git cmake libpistache-dev g++-10 libboost-python-dev libboost-filesystem-dev libboost-numpy-dev libcurl4-openssl-dev nlohmann-json3-dev libzip-dev cython3 python3-numpy libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev doxygen libgsl-dev libopencv-dev python3-opencv python3-pil
 
RUN apt install -y libgazebo11-dev gazebo11 gazebo11-plugin-base
RUN apt install -y python3-flask python3-flask-cors python3-restrictedpython uwsgi-core uwsgi-plugin-python3

ADD build.sh /root
ADD export_nrp.sh /root

ADD nrp-core /root/nrp-core
WORKDIR /root
ARG NRP_INSTALL_PATH=/NRP/nrp
RUN mkdir -p $NRP_INSTALL_PATH

RUN /bin/bash -c "source /opt/ros/noetic/setup.bash; source build.sh"
#RUN cp -r /root/.local/nrp /NRP
ARG NRP_PATH=/NRP/nrp
RUN /bin/bash -c "echo 'export PYTHONPATH='$NRP_PATH'/lib/python3.8/site-packages:\$PYTHONPATH' >> $HOME/.bashrc"
RUN /bin/bash -c "echo 'export LD_LIBRARY_PATH='$NRP_PATH'/lib:\$LD_LIBRARY_PATH' >> $HOME/.bashrc"
RUN /bin/bash -c "echo 'export PATH='$NRP_PATH'/bin:\$PATH' >> $HOME/.bashrc"
RUN /bin/bash -c ". /usr/share/gazebo-11/setup.sh"


# Install Docker
RUN apt-get install -y ca-certificates curl gnupg lsb-release
RUN curl -fsSL https://download.docker.com/linux/ubuntu/gpg |  gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
RUN echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list > /dev/null

RUN apt-get update
RUN apt-get install -y docker-ce docker-ce-cli containerd.io
RUN pip3 install docker
RUN mkdir /Model
ADD opensim /Model/opensim
VOLUME ["/Model"]
sudo apt-get update
sudo apt-get --yes install git cmake cmake-curses-gui \
                           freeglut3-dev libxi-dev libxmu-dev \
                           liblapack-dev swig python-dev \
                           openjdk-8-jdk
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
git clone https://github.com/opensim-org/opensim-core.git
mkdir opensim_dependencies_build
cd opensim_dependencies_build
cmake ../opensim-core/dependencies/ \
      -DCMAKE_INSTALL_PREFIX='../opensim_dependencies_install' \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo
make -j8
cd ..
mkdir opensim_build
cd opensim_build
cmake ../opensim-core \
      -DCMAKE_INSTALL_PREFIX="../opensim_install" \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DOPENSIM_DEPENDENCIES_DIR="../opensim_dependencies_install" \
      -DBUILD_PYTHON_WRAPPING=ON \
      -DBUILD_JAVA_WRAPPING=ON \
      -DWITH_BTK=ON
make -j8
ctest -j8
make -j8 install

#echo 'export PATH=~/OwnApp/opensim_install/bin:$PATH' >> ~/.bashrc

cd ../opensim_install/lib/python3.8/site-packages
sudo python3 setup.py install


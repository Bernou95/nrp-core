cd nrp-core
[ -e build ] && sudo rm -r build
mkdir build

cd build
export C=/usr/bin/gcc-10; export CXX=/usr/bin/g++-10
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local/nrp

[ -e /usr/local/nrp ] && sudo rm -r /usr/local/nrp
sudo mkdir -p /usr/local/nrp
sudo chown 777 /usr/local/nrp
sudo mkdir -p /usr/local/nrp/share/doc/nest

sudo make -j8

sudo make install

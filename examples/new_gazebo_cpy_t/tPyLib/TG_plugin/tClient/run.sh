if [ -e build ]
then
  rm -r build
fi

mkdir build
cd build

cmake ..
make

cp ../t.py bin
cd bin

export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:$PWD
echo $GAZEBO_PLUGIN_PATH
python3 t.py


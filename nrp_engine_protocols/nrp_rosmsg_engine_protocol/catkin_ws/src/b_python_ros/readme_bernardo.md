You should be able to use this package with the following lines once you are in .../catkin_ws/src/boost_python_ros_scripts

	./generate_pkg_bindings.py --package=std_msgs --cpp_target_dir=../out/cpp --py_target_dir=../out/py

	./generate_pkg_bindings.py --package=geometry_msgs --cpp_target_dir=../out/geometry_msgs/cpp --py_target_dir=../out/geometry_msgs/py

	./generate_pkg_bindings.py --package=nav_msgs --cpp_target_dir=../out/nav_msgs/cpp --py_target_dir=../out/nav_msgs/py

	./generate_pkg_bindings.py --package=sensor_msgs --cpp_target_dir=../out/sensor_msgs/cpp --py_target_dir=../out/sensor_msgs/py


Make sure to compile using catkin_make on ~/catkin_ws/ before trying to use this package

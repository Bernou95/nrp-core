#!/usr/bin/env python3

# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2023 NRP Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This project has received funding from the European Union’s Horizon 2020
# Framework Programme for Research and Innovation under the Specific Grant
# Agreement No. 945539 (Human Brain Project SGA3).
#
# This file is a modified version of another one originally authored by:
# Bhaskara Marthi with license BSD.
# The original file can be found at: https://github.com/bhaskara/boost_python_ros


import os
import os.path
import code_generation as cg
import optparse


if __name__ == "__main__":
    parser = optparse.OptionParser(epilog='Generates boost python bindings for ROS 2 msgs from a particular ROS 2 package.')
    parser.add_option('--package', metavar='ROS_PACKAGE',  dest='package', type='string',
                       help='The ROS 2 package to generate bindings for.')
    parser.add_option('--cpp_target_dir', metavar='CPP_TARGET_DIR', dest='cpp_target_dir', type='string',
                        default= os.getcwd(), help='Where to place the generated cpp files.')
    parser.add_option('--py_target_dir',metavar='PY_TARGET_DIR', dest='py_target_dir', type='string',
                        default= os.getcwd(), help='Where to place the ROS_PACKAGE python converter files.')
    parser.add_option('--msg_types_file', metavar='MSG_TYPES_FILE', dest='msg_types_file', type='string',
                      help='File to which the C++ message types of the package are appended.')
    parser.add_option('--msg_headers_file', metavar='MSG_HEADERS_FILE', dest='msg_headers_file', type='string',
                      help='File to which the C++ message headers of the package are appended.')
    (options, a) = parser.parse_args()

    pkg = options.package
    cpp_target_dir = options.cpp_target_dir
    py_target_dir = options.py_target_dir
    if not os.path.exists(py_target_dir):
        os.makedirs(py_target_dir)
    if not os.path.exists(cpp_target_dir):
        os.makedirs(cpp_target_dir)
    cg.write_bindings(pkg, cpp_target_dir)
    cg.write_rclpy_conversions(pkg, py_target_dir)
    if options.msg_types_file and options.msg_headers_file:
        cg.write_msg_lists(pkg, options.msg_types_file, options.msg_headers_file)

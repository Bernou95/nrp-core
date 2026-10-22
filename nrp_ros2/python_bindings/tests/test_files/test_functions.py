#!/usr/bin/python3

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

import unittest
import numpy as np
import nrp_core.data.nrp_ros.builtin_interfaces as bi
import nrp_core.data.nrp_ros.std_msgs as smb
import nrp_core.data.nrp_ros.geometry_msgs as gmb
import nrp_core.data.nrp_ros.sensor_msgs as snb
import nrp_core.data.nrp_ros.nrp_ros_msgs as nrm



def test_input(input_rosmsg):

    # Create a test case object, so that we can use unittest assertions

    tc = unittest.TestCase()

    # Test that all basic types of objects were translated
    # correctly from Test.msg into python objects


    tc.assertTrue       (input_rosmsg.bool_msg)

    tc.assertEqual      (input_rosmsg.int8_msg, 20)
    tc.assertEqual      (input_rosmsg.uint8_msg, 21)
    tc.assertEqual      (input_rosmsg.int16_msg, 22)
    tc.assertEqual      (input_rosmsg.uint16_msg, 23)
    tc.assertEqual      (input_rosmsg.int32_msg, 24)
    tc.assertEqual      (input_rosmsg.uint32_msg, 25)
    tc.assertEqual      (input_rosmsg.int64_msg, 26)
    tc.assertEqual      (input_rosmsg.uint64_msg, 27)

    tc.assertAlmostEqual(input_rosmsg.float32_msg, np.float32(100.13))
    tc.assertAlmostEqual(input_rosmsg.float64_msg, 56.56)
    tc.assertEqual      (input_rosmsg.string_msg, "Armageddon")

    tc.assertEqual      (list(input_rosmsg.byte_array_msg), [1, 0, 1, 0])
    tc.assertEqual      (list(input_rosmsg.float_array_msg), [1, 0, 2])
    tc.assertEqual      (list(input_rosmsg.fixed_array_msg), [7, 8, 9])
    tc.assertEqual      (input_rosmsg.fixed_array_msg[-1], 9)
    tc.assertEqual      (len(input_rosmsg.fixed_array_msg), 3)
    tc.assertEqual      (list(input_rosmsg.bool_array_msg), [True, False, True])

    duration = bi.Duration()
    tc.assertEqual      (input_rosmsg.duration_msg.sec, duration.sec)
    tc.assertEqual      (input_rosmsg.duration_msg.nanosec, duration.nanosec)

    time = bi.Time()
    tc.assertEqual      (input_rosmsg.time_msg.sec, time.sec)
    tc.assertEqual      (input_rosmsg.time_msg.nanosec, time.nanosec)

    tc.assertEqual      (input_rosmsg.std_string_msg.data, "TestString")

    pose = gmb.Pose()
    tc.assertEqual      (input_rosmsg.pose_msg[0].position.x, pose.position.x)
    tc.assertEqual      (input_rosmsg.pose_msg[0].position.y, pose.position.x)
    tc.assertEqual      (input_rosmsg.pose_msg[0].position.z, pose.position.x)

    tc.assertEqual      (input_rosmsg.pose_msg[0].orientation.x, pose.orientation.x)
    tc.assertEqual      (input_rosmsg.pose_msg[0].orientation.y, pose.orientation.y)
    tc.assertEqual      (input_rosmsg.pose_msg[0].orientation.z, pose.orientation.z)
    tc.assertEqual      (input_rosmsg.pose_msg[0].orientation.w, pose.orientation.w)



def test_output():

    # Fill the test datapack with different types of data
    # The C++ caller code will test that they were translated
    # correctly from python objects into Test.msg

    test_datapack = nrm.Test()

    # Standard python types

    test_datapack.bool_msg       = True
    test_datapack.int8_msg       = 1
    test_datapack.uint8_msg      = 2
    test_datapack.int16_msg      = 3
    test_datapack.uint16_msg     = 4
    test_datapack.int32_msg      = 5
    test_datapack.uint32_msg     = 6
    test_datapack.int64_msg      = 7
    test_datapack.uint64_msg     = 8

    test_datapack.float32_msg             = np.float32(43.21)
    test_datapack.float64_msg             = 41.21
    test_datapack.string_msg              = "string"
    test_datapack.std_string_msg.data     = "test"

    test_datapack.byte_array_msg.append(1)
    test_datapack.byte_array_msg.append(0)
    test_datapack.byte_array_msg.append(1)

    test_datapack.float_array_msg.append(4)
    test_datapack.float_array_msg.append(1)
    test_datapack.float_array_msg.append(6)

    # Sequences can also be assigned directly
    test_datapack.fixed_array_msg = np.array([1.0, 2.0, 3.0])
    test_datapack.bool_array_msg = [False, True]

    test_datapack.duration_msg.sec = 60
    test_datapack.duration_msg.nanosec = 0

    test_datapack.time_msg.sec = 0
    test_datapack.time_msg.nanosec = 1000000

    pose = gmb.Pose()
    pose.position.x = 1
    pose.position.y = 2
    pose.position.z = 3

    pose.orientation.x = 1
    pose.orientation.y = 2
    pose.orientation.z = 3
    pose.orientation.w = 4

    test_datapack.pose_msg.append(pose)

    return test_datapack

def test_unsupported_not_default_constructor_duration(input_datapack):
    # Try to use not default constructor
    # Should throw. The exception should be caught by C++ part of the test

    input_datapack.duration_msg = bi.Duration(60)

def test_unsupported_not_default_constructor_time(input_datapack):
    # Try to use not default constructor
    # Should throw. The exception should be caught by C++ part of the test

    input_datapack.time_msg = bi.Time(1)

def test_wrong_fixed_array_length(input_datapack):
    # Should throw. The exception should be caught by C++ part of the test

    input_datapack.fixed_array_msg = [1.0, 2.0]


def test_standard_messages():

    tc = unittest.TestCase()

    # Fixed size arrays
    imu = snb.Imu()
    tc.assertEqual(len(imu.orientation_covariance), 9)
    imu.orientation_covariance[4] = 0.5
    tc.assertEqual(imu.orientation_covariance[4], 0.5)
    with tc.assertRaises(IndexError):
        imu.orientation_covariance[9]

    # Sequences of primitive types and nested messages
    img = snb.Image()
    img.header.frame_id = "camera"
    img.header.stamp.sec = 3
    img.height = 2
    img.width = 3
    img.encoding = "rgb8"
    img.data = bytes(range(18))
    tc.assertEqual(len(img.data), 18)
    tc.assertEqual(img.data[17], 17)

    # Constants
    tc.assertEqual(snb.PointField.FLOAT32, 7)
    tc.assertEqual(snb.NavSatStatus.STATUS_NO_FIX, -1)

    # Printing uses the ROS 2 YAML representation
    tc.assertIn('frame_id: "camera"', str(img))

    # Message types used in vectors of other packages
    path = gmb.PoseArray()
    path.poses.append(gmb.Pose())
    path.poses[0].position.x = 2.5
    tc.assertEqual(path.poses[0].position.x, 2.5)

    # DataPacks
    dp = snb.ImageDataPack("camera", "engine")
    dp.data.width = 5
    tc.assertEqual(dp.data.width, 5)


def test_rclpy_conversions():
    import nrp_core.data.nrp_ros.nrp_ros_msgs_conversions as nrc
    import nrp_core.data.nrp_ros.sensor_msgs_conversions as snc
    import nrp_core.data.nrp_ros.std_msgs_conversions as stc
    import nrp_ros_msgs.msg
    import sensor_msgs.msg
    import std_msgs.msg

    tc = unittest.TestCase()

    # boost -> rclpy -> boost
    t = test_output()
    ros = nrc.Test_to_ros(t)
    tc.assertIsInstance(ros, nrp_ros_msgs.msg.Test)
    tc.assertEqual(ros.string_msg, "string")
    tc.assertEqual(list(ros.byte_array_msg), [1, 0, 1])
    tc.assertEqual(list(ros.fixed_array_msg), [1, 2, 3])
    tc.assertEqual(list(ros.bool_array_msg), [False, True])
    tc.assertEqual(ros.time_msg.nanosec, 1000000)
    tc.assertEqual(ros.pose_msg[0].orientation.w, 4)

    back = nrc.Test_to_boost(ros)
    tc.assertIsInstance(back, nrm.Test)
    tc.assertEqual(back.uint64_msg, 8)
    tc.assertEqual(list(back.float_array_msg), [4, 1, 6])
    tc.assertEqual(list(back.fixed_array_msg), [1, 2, 3])
    tc.assertEqual(back.pose_msg[0].position.z, 3)

    # rclpy -> boost
    img = sensor_msgs.msg.Image(height=1, width=2, encoding="mono8", data=bytes([10, 20]))
    img.header.frame_id = "cam"
    bimg = snc.Image_to_boost(img)
    tc.assertEqual(list(bimg.data), [10, 20])
    tc.assertEqual(bimg.header.frame_id, "cam")
    tc.assertEqual(snc.Image_to_ros(bimg), img)

    # byte fields are returned as bytes by rclpy
    tc.assertEqual(stc.Byte_to_boost(std_msgs.msg.Byte(data=5)).data, 5)

    # Wrong types
    with tc.assertRaises(TypeError):
        nrc.Test_to_ros(img)
    with tc.assertRaises(TypeError):
        snc.Image_to_boost(t)



# EOF

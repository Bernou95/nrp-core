import roslib; roslib.load_manifest('None')
import rostime_boost_python

import std_msgs.msg
import std_msgs_cpp as cpp

def UInt64_to_ros(x):
    m = std_msgs.msg.UInt64()
    m.data = x.data
    return m

cpp.UInt64.to_ros = UInt64_to_ros

def UInt64_to_boost(m):
    x = cpp.UInt64()
    x.data = m.data
    return x

std_msgs.msg.UInt64.to_boost = UInt64_to_boost

def UInt8MultiArray_to_ros(x):
    m = std_msgs.msg.UInt8MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.UInt8MultiArray.to_ros = UInt8MultiArray_to_ros

def UInt8MultiArray_to_boost(m):
    x = cpp.UInt8MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.UInt8MultiArray.to_boost = UInt8MultiArray_to_boost

def Float64_to_ros(x):
    m = std_msgs.msg.Float64()
    m.data = x.data
    return m

cpp.Float64.to_ros = Float64_to_ros

def Float64_to_boost(m):
    x = cpp.Float64()
    x.data = m.data
    return x

std_msgs.msg.Float64.to_boost = Float64_to_boost

def Bool_to_ros(x):
    m = std_msgs.msg.Bool()
    m.data = x.data
    return m

cpp.Bool.to_ros = Bool_to_ros

def Bool_to_boost(m):
    x = cpp.Bool()
    x.data = m.data
    return x

std_msgs.msg.Bool.to_boost = Bool_to_boost

def UInt8_to_ros(x):
    m = std_msgs.msg.UInt8()
    m.data = x.data
    return m

cpp.UInt8.to_ros = UInt8_to_ros

def UInt8_to_boost(m):
    x = cpp.UInt8()
    x.data = m.data
    return x

std_msgs.msg.UInt8.to_boost = UInt8_to_boost

def ColorRGBA_to_ros(x):
    m = std_msgs.msg.ColorRGBA()
    m.r = x.r
    m.g = x.g
    m.b = x.b
    m.a = x.a
    return m

cpp.ColorRGBA.to_ros = ColorRGBA_to_ros

def ColorRGBA_to_boost(m):
    x = cpp.ColorRGBA()
    x.r = m.r
    x.g = m.g
    x.b = m.b
    x.a = m.a
    return x

std_msgs.msg.ColorRGBA.to_boost = ColorRGBA_to_boost

def ByteMultiArray_to_ros(x):
    m = std_msgs.msg.ByteMultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.ByteMultiArray.to_ros = ByteMultiArray_to_ros

def ByteMultiArray_to_boost(m):
    x = cpp.ByteMultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.ByteMultiArray.to_boost = ByteMultiArray_to_boost

def Int64_to_ros(x):
    m = std_msgs.msg.Int64()
    m.data = x.data
    return m

cpp.Int64.to_ros = Int64_to_ros

def Int64_to_boost(m):
    x = cpp.Int64()
    x.data = m.data
    return x

std_msgs.msg.Int64.to_boost = Int64_to_boost

def Int32_to_ros(x):
    m = std_msgs.msg.Int32()
    m.data = x.data
    return m

cpp.Int32.to_ros = Int32_to_ros

def Int32_to_boost(m):
    x = cpp.Int32()
    x.data = m.data
    return x

std_msgs.msg.Int32.to_boost = Int32_to_boost

def MultiArrayDimension_to_ros(x):
    m = std_msgs.msg.MultiArrayDimension()
    m.label = x.label
    m.size = x.size
    m.stride = x.stride
    return m

cpp.MultiArrayDimension.to_ros = MultiArrayDimension_to_ros

def MultiArrayDimension_to_boost(m):
    x = cpp.MultiArrayDimension()
    x.label = m.label
    x.size = m.size
    x.stride = m.stride
    return x

std_msgs.msg.MultiArrayDimension.to_boost = MultiArrayDimension_to_boost

def Int16MultiArray_to_ros(x):
    m = std_msgs.msg.Int16MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.Int16MultiArray.to_ros = Int16MultiArray_to_ros

def Int16MultiArray_to_boost(m):
    x = cpp.Int16MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.Int16MultiArray.to_boost = Int16MultiArray_to_boost

def Float32MultiArray_to_ros(x):
    m = std_msgs.msg.Float32MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.Float32MultiArray.to_ros = Float32MultiArray_to_ros

def Float32MultiArray_to_boost(m):
    x = cpp.Float32MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.Float32MultiArray.to_boost = Float32MultiArray_to_boost

def Char_to_ros(x):
    m = std_msgs.msg.Char()
    m.data = x.data
    return m

cpp.Char.to_ros = Char_to_ros

def Char_to_boost(m):
    x = cpp.Char()
    x.data = m.data
    return x

std_msgs.msg.Char.to_boost = Char_to_boost

def Int8_to_ros(x):
    m = std_msgs.msg.Int8()
    m.data = x.data
    return m

cpp.Int8.to_ros = Int8_to_ros

def Int8_to_boost(m):
    x = cpp.Int8()
    x.data = m.data
    return x

std_msgs.msg.Int8.to_boost = Int8_to_boost

def UInt16MultiArray_to_ros(x):
    m = std_msgs.msg.UInt16MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.UInt16MultiArray.to_ros = UInt16MultiArray_to_ros

def UInt16MultiArray_to_boost(m):
    x = cpp.UInt16MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.UInt16MultiArray.to_boost = UInt16MultiArray_to_boost

def Empty_to_ros(x):
    m = std_msgs.msg.Empty()
    return m

cpp.Empty.to_ros = Empty_to_ros

def Empty_to_boost(m):
    x = cpp.Empty()
    return x

std_msgs.msg.Empty.to_boost = Empty_to_boost

def String_to_ros(x):
    m = std_msgs.msg.String()
    m.data = x.data
    return m

cpp.String.to_ros = String_to_ros

def String_to_boost(m):
    x = cpp.String()
    x.data = m.data
    return x

std_msgs.msg.String.to_boost = String_to_boost

def UInt16_to_ros(x):
    m = std_msgs.msg.UInt16()
    m.data = x.data
    return m

cpp.UInt16.to_ros = UInt16_to_ros

def UInt16_to_boost(m):
    x = cpp.UInt16()
    x.data = m.data
    return x

std_msgs.msg.UInt16.to_boost = UInt16_to_boost

def Int8MultiArray_to_ros(x):
    m = std_msgs.msg.Int8MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.Int8MultiArray.to_ros = Int8MultiArray_to_ros

def Int8MultiArray_to_boost(m):
    x = cpp.Int8MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.Int8MultiArray.to_boost = Int8MultiArray_to_boost

def MultiArrayLayout_to_ros(x):
    m = std_msgs.msg.MultiArrayLayout()
    m.dim = [y.to_ros() for y in x.dim]
    m.data_offset = x.data_offset
    return m

cpp.MultiArrayLayout.to_ros = MultiArrayLayout_to_ros

def MultiArrayLayout_to_boost(m):
    x = cpp.MultiArrayLayout()
    x.dim[:] = [y.to_boost() for y in m.dim]
    x.data_offset = m.data_offset
    return x

std_msgs.msg.MultiArrayLayout.to_boost = MultiArrayLayout_to_boost

def UInt32MultiArray_to_ros(x):
    m = std_msgs.msg.UInt32MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.UInt32MultiArray.to_ros = UInt32MultiArray_to_ros

def UInt32MultiArray_to_boost(m):
    x = cpp.UInt32MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.UInt32MultiArray.to_boost = UInt32MultiArray_to_boost

def Float32_to_ros(x):
    m = std_msgs.msg.Float32()
    m.data = x.data
    return m

cpp.Float32.to_ros = Float32_to_ros

def Float32_to_boost(m):
    x = cpp.Float32()
    x.data = m.data
    return x

std_msgs.msg.Float32.to_boost = Float32_to_boost

def Int16_to_ros(x):
    m = std_msgs.msg.Int16()
    m.data = x.data
    return m

cpp.Int16.to_ros = Int16_to_ros

def Int16_to_boost(m):
    x = cpp.Int16()
    x.data = m.data
    return x

std_msgs.msg.Int16.to_boost = Int16_to_boost

def Int32MultiArray_to_ros(x):
    m = std_msgs.msg.Int32MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.Int32MultiArray.to_ros = Int32MultiArray_to_ros

def Int32MultiArray_to_boost(m):
    x = cpp.Int32MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.Int32MultiArray.to_boost = Int32MultiArray_to_boost

def Header_to_ros(x):
    m = std_msgs.msg.Header()
    m.seq = x.seq
    m.stamp.secs = x.stamp.sec
    m.stamp.nsecs = x.stamp.nsec
    m.frame_id = x.frame_id
    return m

cpp.Header.to_ros = Header_to_ros

def Header_to_boost(m):
    x = cpp.Header()
    x.seq = m.seq
    x.stamp.sec = m.stamp.secs
    x.stamp.nsec = m.stamp.nsecs
    x.frame_id = m.frame_id
    return x

std_msgs.msg.Header.to_boost = Header_to_boost

def Duration_to_ros(x):
    m = std_msgs.msg.Duration()
    m.data.secs = x.data.sec
    m.data.nsecs = x.data.nsec
    return m

cpp.Duration.to_ros = Duration_to_ros

def Duration_to_boost(m):
    x = cpp.Duration()
    x.data.sec = m.data.secs
    x.data.nsec = m.data.nsecs
    return x

std_msgs.msg.Duration.to_boost = Duration_to_boost

def Time_to_ros(x):
    m = std_msgs.msg.Time()
    m.data.secs = x.data.sec
    m.data.nsecs = x.data.nsec
    return m

cpp.Time.to_ros = Time_to_ros

def Time_to_boost(m):
    x = cpp.Time()
    x.data.sec = m.data.secs
    x.data.nsec = m.data.nsecs
    return x

std_msgs.msg.Time.to_boost = Time_to_boost

def Int64MultiArray_to_ros(x):
    m = std_msgs.msg.Int64MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.Int64MultiArray.to_ros = Int64MultiArray_to_ros

def Int64MultiArray_to_boost(m):
    x = cpp.Int64MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.Int64MultiArray.to_boost = Int64MultiArray_to_boost

def UInt64MultiArray_to_ros(x):
    m = std_msgs.msg.UInt64MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.UInt64MultiArray.to_ros = UInt64MultiArray_to_ros

def UInt64MultiArray_to_boost(m):
    x = cpp.UInt64MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.UInt64MultiArray.to_boost = UInt64MultiArray_to_boost

def UInt32_to_ros(x):
    m = std_msgs.msg.UInt32()
    m.data = x.data
    return m

cpp.UInt32.to_ros = UInt32_to_ros

def UInt32_to_boost(m):
    x = cpp.UInt32()
    x.data = m.data
    return x

std_msgs.msg.UInt32.to_boost = UInt32_to_boost

def Float64MultiArray_to_ros(x):
    m = std_msgs.msg.Float64MultiArray()
    m.layout = x.layout.to_ros()
    m.data = x.data
    return m

cpp.Float64MultiArray.to_ros = Float64MultiArray_to_ros

def Float64MultiArray_to_boost(m):
    x = cpp.Float64MultiArray()
    x.layout = m.layout.to_boost()
    x.data = m.data
    return x

std_msgs.msg.Float64MultiArray.to_boost = Float64MultiArray_to_boost

def Byte_to_ros(x):
    m = std_msgs.msg.Byte()
    m.data = x.data
    return m

cpp.Byte.to_ros = Byte_to_ros

def Byte_to_boost(m):
    x = cpp.Byte()
    x.data = m.data
    return x

std_msgs.msg.Byte.to_boost = Byte_to_boost

import roslib; roslib.load_manifest('boost_python_ros')
import rostime_boost_python
import std_msgs_boost
import geometry_msgs_boost
import sensor_msgs.msg
import sensor_msgs_cpp as cpp

def PointCloud2_to_ros(x):
    m = sensor_msgs.msg.PointCloud2()
    m.header = x.header.to_ros()
    m.height = x.height
    m.width = x.width
    m.fields = [y.to_ros() for y in x.fields]
    m.is_bigendian = x.is_bigendian
    m.point_step = x.point_step
    m.row_step = x.row_step
    m.data = x.data
    m.is_dense = x.is_dense
    return m

cpp.PointCloud2.to_ros = PointCloud2_to_ros

def PointCloud2_to_boost(m):
    x = cpp.PointCloud2()
    x.header = m.header.to_boost()
    x.height = m.height
    x.width = m.width
    x.fields[:] = [y.to_boost() for y in m.fields]
    x.is_bigendian = m.is_bigendian
    x.point_step = m.point_step
    x.row_step = m.row_step
    x.data = m.data
    x.is_dense = m.is_dense
    return x

sensor_msgs.msg.PointCloud2.to_boost = PointCloud2_to_boost

def NavSatFix_to_ros(x):
    m = sensor_msgs.msg.NavSatFix()
    m.header = x.header.to_ros()
    m.status = x.status.to_ros()
    m.latitude = x.latitude
    m.longitude = x.longitude
    m.altitude = x.altitude
    m.position_covariance = x.position_covariance
    m.position_covariance_type = x.position_covariance_type
    return m

cpp.NavSatFix.to_ros = NavSatFix_to_ros

def NavSatFix_to_boost(m):
    x = cpp.NavSatFix()
    x.header = m.header.to_boost()
    x.status = m.status.to_boost()
    x.latitude = m.latitude
    x.longitude = m.longitude
    x.altitude = m.altitude
    x.position_covariance = m.position_covariance
    x.position_covariance_type = m.position_covariance_type
    return x

sensor_msgs.msg.NavSatFix.to_boost = NavSatFix_to_boost

def Illuminance_to_ros(x):
    m = sensor_msgs.msg.Illuminance()
    m.header = x.header.to_ros()
    m.illuminance = x.illuminance
    m.variance = x.variance
    return m

cpp.Illuminance.to_ros = Illuminance_to_ros

def Illuminance_to_boost(m):
    x = cpp.Illuminance()
    x.header = m.header.to_boost()
    x.illuminance = m.illuminance
    x.variance = m.variance
    return x

sensor_msgs.msg.Illuminance.to_boost = Illuminance_to_boost

def CameraInfo_to_ros(x):
    m = sensor_msgs.msg.CameraInfo()
    m.header = x.header.to_ros()
    m.height = x.height
    m.width = x.width
    m.distortion_model = x.distortion_model
    m.D = x.D
    m.K = x.K
    m.R = x.R
    m.P = x.P
    m.binning_x = x.binning_x
    m.binning_y = x.binning_y
    m.roi = x.roi.to_ros()
    return m

cpp.CameraInfo.to_ros = CameraInfo_to_ros

def CameraInfo_to_boost(m):
    x = cpp.CameraInfo()
    x.header = m.header.to_boost()
    x.height = m.height
    x.width = m.width
    x.distortion_model = m.distortion_model
    x.D = m.D
    x.K = m.K
    x.R = m.R
    x.P = m.P
    x.binning_x = m.binning_x
    x.binning_y = m.binning_y
    x.roi = m.roi.to_boost()
    return x

sensor_msgs.msg.CameraInfo.to_boost = CameraInfo_to_boost

def Joy_to_ros(x):
    m = sensor_msgs.msg.Joy()
    m.header = x.header.to_ros()
    m.axes = x.axes
    m.buttons = x.buttons
    return m

cpp.Joy.to_ros = Joy_to_ros

def Joy_to_boost(m):
    x = cpp.Joy()
    x.header = m.header.to_boost()
    x.axes = m.axes
    x.buttons = m.buttons
    return x

sensor_msgs.msg.Joy.to_boost = Joy_to_boost

def ChannelFloat32_to_ros(x):
    m = sensor_msgs.msg.ChannelFloat32()
    m.name = x.name
    m.values = x.values
    return m

cpp.ChannelFloat32.to_ros = ChannelFloat32_to_ros

def ChannelFloat32_to_boost(m):
    x = cpp.ChannelFloat32()
    x.name = m.name
    x.values = m.values
    return x

sensor_msgs.msg.ChannelFloat32.to_boost = ChannelFloat32_to_boost

def JointState_to_ros(x):
    m = sensor_msgs.msg.JointState()
    m.header = x.header.to_ros()
    m.name = x.name
    m.position = x.position
    m.velocity = x.velocity
    m.effort = x.effort
    return m

cpp.JointState.to_ros = JointState_to_ros

def JointState_to_boost(m):
    x = cpp.JointState()
    x.header = m.header.to_boost()
    x.name = m.name
    x.position = m.position
    x.velocity = m.velocity
    x.effort = m.effort
    return x

sensor_msgs.msg.JointState.to_boost = JointState_to_boost

def PointField_to_ros(x):
    m = sensor_msgs.msg.PointField()
    m.name = x.name
    m.offset = x.offset
    m.datatype = x.datatype
    m.count = x.count
    return m

cpp.PointField.to_ros = PointField_to_ros

def PointField_to_boost(m):
    x = cpp.PointField()
    x.name = m.name
    x.offset = m.offset
    x.datatype = m.datatype
    x.count = m.count
    return x

sensor_msgs.msg.PointField.to_boost = PointField_to_boost

def Range_to_ros(x):
    m = sensor_msgs.msg.Range()
    m.header = x.header.to_ros()
    m.radiation_type = x.radiation_type
    m.field_of_view = x.field_of_view
    m.min_range = x.min_range
    m.max_range = x.max_range
    m.range = x.range
    return m

cpp.Range.to_ros = Range_to_ros

def Range_to_boost(m):
    x = cpp.Range()
    x.header = m.header.to_boost()
    x.radiation_type = m.radiation_type
    x.field_of_view = m.field_of_view
    x.min_range = m.min_range
    x.max_range = m.max_range
    x.range = m.range
    return x

sensor_msgs.msg.Range.to_boost = Range_to_boost

def MultiDOFJointState_to_ros(x):
    m = sensor_msgs.msg.MultiDOFJointState()
    m.header = x.header.to_ros()
    m.joint_names = x.joint_names
    m.transforms = [y.to_ros() for y in x.transforms]
    m.twist = [y.to_ros() for y in x.twist]
    m.wrench = [y.to_ros() for y in x.wrench]
    return m

cpp.MultiDOFJointState.to_ros = MultiDOFJointState_to_ros

def MultiDOFJointState_to_boost(m):
    x = cpp.MultiDOFJointState()
    x.header = m.header.to_boost()
    x.joint_names = m.joint_names
    x.transforms[:] = [y.to_boost() for y in m.transforms]
    x.twist[:] = [y.to_boost() for y in m.twist]
    x.wrench[:] = [y.to_boost() for y in m.wrench]
    return x

sensor_msgs.msg.MultiDOFJointState.to_boost = MultiDOFJointState_to_boost

def Imu_to_ros(x):
    m = sensor_msgs.msg.Imu()
    m.header = x.header.to_ros()
    m.orientation = x.orientation.to_ros()
    m.orientation_covariance = x.orientation_covariance
    m.angular_velocity = x.angular_velocity.to_ros()
    m.angular_velocity_covariance = x.angular_velocity_covariance
    m.linear_acceleration = x.linear_acceleration.to_ros()
    m.linear_acceleration_covariance = x.linear_acceleration_covariance
    return m

cpp.Imu.to_ros = Imu_to_ros

def Imu_to_boost(m):
    x = cpp.Imu()
    x.header = m.header.to_boost()
    x.orientation = m.orientation.to_boost()
    x.orientation_covariance = m.orientation_covariance
    x.angular_velocity = m.angular_velocity.to_boost()
    x.angular_velocity_covariance = m.angular_velocity_covariance
    x.linear_acceleration = m.linear_acceleration.to_boost()
    x.linear_acceleration_covariance = m.linear_acceleration_covariance
    return x

sensor_msgs.msg.Imu.to_boost = Imu_to_boost

def TimeReference_to_ros(x):
    m = sensor_msgs.msg.TimeReference()
    m.header = x.header.to_ros()
    m.time_ref.secs = x.time_ref.sec
    m.time_ref.nsecs = x.time_ref.nsec
    m.source = x.source
    return m

cpp.TimeReference.to_ros = TimeReference_to_ros

def TimeReference_to_boost(m):
    x = cpp.TimeReference()
    x.header = m.header.to_boost()
    x.time_ref.sec = m.time_ref.secs
    x.time_ref.nsec = m.time_ref.nsecs
    x.source = m.source
    return x

sensor_msgs.msg.TimeReference.to_boost = TimeReference_to_boost

def LaserEcho_to_ros(x):
    m = sensor_msgs.msg.LaserEcho()
    m.echoes = x.echoes
    return m

cpp.LaserEcho.to_ros = LaserEcho_to_ros

def LaserEcho_to_boost(m):
    x = cpp.LaserEcho()
    x.echoes = m.echoes
    return x

sensor_msgs.msg.LaserEcho.to_boost = LaserEcho_to_boost

def CompressedImage_to_ros(x):
    m = sensor_msgs.msg.CompressedImage()
    m.header = x.header.to_ros()
    m.format = x.format
    m.data = x.data
    return m

cpp.CompressedImage.to_ros = CompressedImage_to_ros

def CompressedImage_to_boost(m):
    x = cpp.CompressedImage()
    x.header = m.header.to_boost()
    x.format = m.format
    x.data = m.data
    return x

sensor_msgs.msg.CompressedImage.to_boost = CompressedImage_to_boost

def NavSatStatus_to_ros(x):
    m = sensor_msgs.msg.NavSatStatus()
    m.status = x.status
    m.service = x.service
    return m

cpp.NavSatStatus.to_ros = NavSatStatus_to_ros

def NavSatStatus_to_boost(m):
    x = cpp.NavSatStatus()
    x.status = m.status
    x.service = m.service
    return x

sensor_msgs.msg.NavSatStatus.to_boost = NavSatStatus_to_boost

def MagneticField_to_ros(x):
    m = sensor_msgs.msg.MagneticField()
    m.header = x.header.to_ros()
    m.magnetic_field = x.magnetic_field.to_ros()
    m.magnetic_field_covariance = x.magnetic_field_covariance
    return m

cpp.MagneticField.to_ros = MagneticField_to_ros

def MagneticField_to_boost(m):
    x = cpp.MagneticField()
    x.header = m.header.to_boost()
    x.magnetic_field = m.magnetic_field.to_boost()
    x.magnetic_field_covariance = m.magnetic_field_covariance
    return x

sensor_msgs.msg.MagneticField.to_boost = MagneticField_to_boost

def RelativeHumidity_to_ros(x):
    m = sensor_msgs.msg.RelativeHumidity()
    m.header = x.header.to_ros()
    m.relative_humidity = x.relative_humidity
    m.variance = x.variance
    return m

cpp.RelativeHumidity.to_ros = RelativeHumidity_to_ros

def RelativeHumidity_to_boost(m):
    x = cpp.RelativeHumidity()
    x.header = m.header.to_boost()
    x.relative_humidity = m.relative_humidity
    x.variance = m.variance
    return x

sensor_msgs.msg.RelativeHumidity.to_boost = RelativeHumidity_to_boost

def RegionOfInterest_to_ros(x):
    m = sensor_msgs.msg.RegionOfInterest()
    m.x_offset = x.x_offset
    m.y_offset = x.y_offset
    m.height = x.height
    m.width = x.width
    m.do_rectify = x.do_rectify
    return m

cpp.RegionOfInterest.to_ros = RegionOfInterest_to_ros

def RegionOfInterest_to_boost(m):
    x = cpp.RegionOfInterest()
    x.x_offset = m.x_offset
    x.y_offset = m.y_offset
    x.height = m.height
    x.width = m.width
    x.do_rectify = m.do_rectify
    return x

sensor_msgs.msg.RegionOfInterest.to_boost = RegionOfInterest_to_boost

def FluidPressure_to_ros(x):
    m = sensor_msgs.msg.FluidPressure()
    m.header = x.header.to_ros()
    m.fluid_pressure = x.fluid_pressure
    m.variance = x.variance
    return m

cpp.FluidPressure.to_ros = FluidPressure_to_ros

def FluidPressure_to_boost(m):
    x = cpp.FluidPressure()
    x.header = m.header.to_boost()
    x.fluid_pressure = m.fluid_pressure
    x.variance = m.variance
    return x

sensor_msgs.msg.FluidPressure.to_boost = FluidPressure_to_boost

def BatteryState_to_ros(x):
    m = sensor_msgs.msg.BatteryState()
    m.header = x.header.to_ros()
    m.voltage = x.voltage
    m.temperature = x.temperature
    m.current = x.current
    m.charge = x.charge
    m.capacity = x.capacity
    m.design_capacity = x.design_capacity
    m.percentage = x.percentage
    m.power_supply_status = x.power_supply_status
    m.power_supply_health = x.power_supply_health
    m.power_supply_technology = x.power_supply_technology
    m.present = x.present
    m.cell_voltage = x.cell_voltage
    m.cell_temperature = x.cell_temperature
    m.location = x.location
    m.serial_number = x.serial_number
    return m

cpp.BatteryState.to_ros = BatteryState_to_ros

def BatteryState_to_boost(m):
    x = cpp.BatteryState()
    x.header = m.header.to_boost()
    x.voltage = m.voltage
    x.temperature = m.temperature
    x.current = m.current
    x.charge = m.charge
    x.capacity = m.capacity
    x.design_capacity = m.design_capacity
    x.percentage = m.percentage
    x.power_supply_status = m.power_supply_status
    x.power_supply_health = m.power_supply_health
    x.power_supply_technology = m.power_supply_technology
    x.present = m.present
    x.cell_voltage = m.cell_voltage
    x.cell_temperature = m.cell_temperature
    x.location = m.location
    x.serial_number = m.serial_number
    return x

sensor_msgs.msg.BatteryState.to_boost = BatteryState_to_boost

def Temperature_to_ros(x):
    m = sensor_msgs.msg.Temperature()
    m.header = x.header.to_ros()
    m.temperature = x.temperature
    m.variance = x.variance
    return m

cpp.Temperature.to_ros = Temperature_to_ros

def Temperature_to_boost(m):
    x = cpp.Temperature()
    x.header = m.header.to_boost()
    x.temperature = m.temperature
    x.variance = m.variance
    return x

sensor_msgs.msg.Temperature.to_boost = Temperature_to_boost

def PointCloud_to_ros(x):
    m = sensor_msgs.msg.PointCloud()
    m.header = x.header.to_ros()
    m.points = [y.to_ros() for y in x.points]
    m.channels = [y.to_ros() for y in x.channels]
    return m

cpp.PointCloud.to_ros = PointCloud_to_ros

def PointCloud_to_boost(m):
    x = cpp.PointCloud()
    x.header = m.header.to_boost()
    x.points[:] = [y.to_boost() for y in m.points]
    x.channels[:] = [y.to_boost() for y in m.channels]
    return x

sensor_msgs.msg.PointCloud.to_boost = PointCloud_to_boost

def LaserScan_to_ros(x):
    m = sensor_msgs.msg.LaserScan()
    m.header = x.header.to_ros()
    m.angle_min = x.angle_min
    m.angle_max = x.angle_max
    m.angle_increment = x.angle_increment
    m.time_increment = x.time_increment
    m.scan_time = x.scan_time
    m.range_min = x.range_min
    m.range_max = x.range_max
    m.ranges = x.ranges
    m.intensities = x.intensities
    return m

cpp.LaserScan.to_ros = LaserScan_to_ros

def LaserScan_to_boost(m):
    x = cpp.LaserScan()
    x.header = m.header.to_boost()
    x.angle_min = m.angle_min
    x.angle_max = m.angle_max
    x.angle_increment = m.angle_increment
    x.time_increment = m.time_increment
    x.scan_time = m.scan_time
    x.range_min = m.range_min
    x.range_max = m.range_max
    x.ranges = m.ranges
    x.intensities = m.intensities
    return x

sensor_msgs.msg.LaserScan.to_boost = LaserScan_to_boost

def JoyFeedbackArray_to_ros(x):
    m = sensor_msgs.msg.JoyFeedbackArray()
    m.array = [y.to_ros() for y in x.array]
    return m

cpp.JoyFeedbackArray.to_ros = JoyFeedbackArray_to_ros

def JoyFeedbackArray_to_boost(m):
    x = cpp.JoyFeedbackArray()
    x.array[:] = [y.to_boost() for y in m.array]
    return x

sensor_msgs.msg.JoyFeedbackArray.to_boost = JoyFeedbackArray_to_boost

def Image_to_ros(x):
    m = sensor_msgs.msg.Image()
    m.header = x.header.to_ros()
    m.height = x.height
    m.width = x.width
    m.encoding = x.encoding
    m.is_bigendian = x.is_bigendian
    m.step = x.step
    m.data = x.data
    return m

cpp.Image.to_ros = Image_to_ros

def Image_to_boost(m):
    x = cpp.Image()
    x.header = m.header.to_boost()
    x.height = m.height
    x.width = m.width
    x.encoding = m.encoding
    x.is_bigendian = m.is_bigendian
    x.step = m.step
    x.data = m.data
    return x

sensor_msgs.msg.Image.to_boost = Image_to_boost

def JoyFeedback_to_ros(x):
    m = sensor_msgs.msg.JoyFeedback()
    m.type = x.type
    m.id = x.id
    m.intensity = x.intensity
    return m

cpp.JoyFeedback.to_ros = JoyFeedback_to_ros

def JoyFeedback_to_boost(m):
    x = cpp.JoyFeedback()
    x.type = m.type
    x.id = m.id
    x.intensity = m.intensity
    return x

sensor_msgs.msg.JoyFeedback.to_boost = JoyFeedback_to_boost

def MultiEchoLaserScan_to_ros(x):
    m = sensor_msgs.msg.MultiEchoLaserScan()
    m.header = x.header.to_ros()
    m.angle_min = x.angle_min
    m.angle_max = x.angle_max
    m.angle_increment = x.angle_increment
    m.time_increment = x.time_increment
    m.scan_time = x.scan_time
    m.range_min = x.range_min
    m.range_max = x.range_max
    m.ranges = [y.to_ros() for y in x.ranges]
    m.intensities = [y.to_ros() for y in x.intensities]
    return m

cpp.MultiEchoLaserScan.to_ros = MultiEchoLaserScan_to_ros

def MultiEchoLaserScan_to_boost(m):
    x = cpp.MultiEchoLaserScan()
    x.header = m.header.to_boost()
    x.angle_min = m.angle_min
    x.angle_max = m.angle_max
    x.angle_increment = m.angle_increment
    x.time_increment = m.time_increment
    x.scan_time = m.scan_time
    x.range_min = m.range_min
    x.range_max = m.range_max
    x.ranges[:] = [y.to_boost() for y in m.ranges]
    x.intensities[:] = [y.to_boost() for y in m.intensities]
    return x

sensor_msgs.msg.MultiEchoLaserScan.to_boost = MultiEchoLaserScan_to_boost

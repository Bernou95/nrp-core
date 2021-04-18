from NRPPythonModule import *
import NRPGazeboDevicesPython
from NRPNestJSONPythonModule import *
import math
#'''
<<<<<<< HEAD
@FromEngineDevice(keyword='time_gazebo', id=DeviceIdentifier('time_gazebo', 'gazebo'))
@FromEngineDevice(keyword='time_nest', id=DeviceIdentifier('time_nest', 'nest'))
@FromEngineDevice(keyword='time_py_1', id=DeviceIdentifier('time_py_1', 'py_1'))
@FromEngineDevice(keyword='time_py_2', id=DeviceIdentifier('time_py_2', 'py_2'))
@TransceiverFunction("client")
def transceiver_function(time_py_1, time_py_2, time_gazebo, time_nest):
    for_gazebo = time_gazebo.timeval/math.pow(10, 9)
    for_nest   = time_nest.data.get("time")/math.pow(10, 9)
    for_py_1   = time_py_1.data.get("time")/math.pow(10, 9)
    for_py_2   = time_py_2.data.get("time")/math.pow(10, 9)

    rec_sync_test = PythonDevice("rec_sync_test", "client")
    #rec_sync_test.data = {"times":tStr}
    rec_sync_test.data = {"times":[for_gazebo, for_nest, for_py_1, for_py_2]}

    return [ rec_sync_test ]
=======
@FromEngineDevice(keyword='device_os_1', id=DeviceIdentifier('device_os_1', 'server_os_1'))
@FromEngineDevice(keyword='device_py', id=DeviceIdentifier('device_py', 'server_py'))
@FromEngineDevice(keyword='test', id=DeviceIdentifier('test', 'nest'))
@TransceiverFunction("client")
def transceiver_function(device_os_1, Check_Time, device_py):
    rec_os_1 = PythonDevice("rec_os_1", "client")
    rec_py = PythonDevice("rec_py", "client")
    rec_os_1.data = device_os_1.data
    rec_py.data = device_py.data
    print("Main: --------------> ")
    print(str(Check_Time.data))
    print("Opensim M1: " + str(rec_os_1.data))
    print("Python    : " + str(rec_py.data))
    return [ rec_os_1, rec_py ]
<<<<<<< HEAD
>>>>>>> dc7adbacd5ccbb81923578be0a04c242f0c3a763
=======
>>>>>>> dc7adbacd5ccbb81923578be0a04c242f0c3a763


from NRPPythonModule import *
import NRPGazeboDevicesPython
from NRPNestJSONPythonModule import *
import math
#'''
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


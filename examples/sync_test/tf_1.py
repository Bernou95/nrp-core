from NRPPythonModule import *
import opensim

#'''
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


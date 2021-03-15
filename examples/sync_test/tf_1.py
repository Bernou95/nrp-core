from NRPPythonModule import *
import opensim

#'''
@SingleTransceiverDevice(keyword='device_os_1', id=DeviceIdentifier('device_os_1', 'servo_os_1'))
@SingleTransceiverDevice(keyword='device_os_2', id=DeviceIdentifier('device_os_2', 'servo_os_2'))
@SingleTransceiverDevice(keyword='device_py', id=DeviceIdentifier('device_py', 'servo_py'))
@TransceiverFunction("client")
def transceiver_function(device_os_1, device_os_2, device_py):
    rec_os_1 = PythonDevice("rec_os_1", "client")
    rec_os_2 = PythonDevice("rec_os_2", "client")
    rec_py = PythonDevice("rec_py", "client")
    rec_os_1.data = device_os_1.data
    rec_os_2.data = device_os_2.data
    rec_py.data = device_py.data
    print("Main: --------------> ")
    print("Opensim M1: " + str(rec_os_1.data))
    print("Opensim M2: " + str(rec_os_2.data))
    print("Python    : " + str(rec_py.data))
    return [ rec_os_1, rec_os_2, rec_py ]


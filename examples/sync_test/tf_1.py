from NRPPythonModule import *
import opensim

#'''
@SingleTransceiverDevice(keyword='device_gz', id=DeviceIdentifier('device_gz', 'servo_gz'))
@SingleTransceiverDevice(keyword='device_os', id=DeviceIdentifier('device_os', 'servo_os'))
@SingleTransceiverDevice(keyword='device_py', id=DeviceIdentifier('device_py', 'servo_py'))
@TransceiverFunction("client")
def transceiver_function(device_gz, device_os, device_py):
    rec_gz = PythonDevice("rec_gz", "client")
    rec_os = PythonDevice("rec_os", "client")
    rec_py = PythonDevice("rec_py", "client")
    rec_gz.data = device_gz.data
    rec_os.data = device_os.data
    rec_py.data = device_py.data
    print("Main: --------------> ")
    print("Gazebo: " + str(rec_gz.data))
    print("Opensim: " + str(rec_os.data))
    print("Python: " + str(rec_py.data))
    return [ rec_gz, rec_os, rec_py ]
'''
@SingleTransceiverDevice(keyword='device_os', id=DeviceIdentifier('device_os', 'servo_os'))
@TransceiverFunction("client")
def transceiver_function(device_os):
    rec_device1 = PythonDevice("rec_os", "client")
    rec_device1.data = device_os.data
    print("Main: ")
    print(rec_device1.data)
    return [ rec_device1 ]
'''

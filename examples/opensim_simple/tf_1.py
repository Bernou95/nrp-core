from NRPPythonModule import *
<<<<<<< HEAD
import opensim
@SingleTransceiverDevice(keyword='device1', id=DeviceIdentifier('device1', 'servo'))
@TransceiverFunction("client")
def transceiver_function(device1):
    rec_device1 = PythonDevice("rec_device1", "client")
    rec_device1.data = device1.data
    print("Main: ")
    print(rec_device1.data)
=======

@SingleTransceiverDevice(keyword='device1', id=DeviceIdentifier('device1', 'python_1'))
@TransceiverFunction("python_2")
def transceiver_function(device1):
    rec_device1 = PythonDevice("rec_device1", "python_2")
    rec_device1.data = device1.data

>>>>>>> 7afeb301645a7fff5737e91503d85bf60c221f37
    return [ rec_device1 ]


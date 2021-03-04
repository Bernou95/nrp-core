from NRPPythonModule import *

@SingleTransceiverDevice(keyword='device2', id=DeviceIdentifier('device2', 'client'))
@TransceiverFunction("servo")
def transceiver_function(device2):
    rec_device2 = PythonDevice("rec_device2", "servo")
    rec_device2.data = device2.data
    print("Main: ")
    print(rec_device2.data)
    return [ rec_device2 ]


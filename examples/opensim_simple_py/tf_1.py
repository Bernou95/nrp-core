from NRPPythonModule import *

@FromEngineDevice(keyword='device1', id=DeviceIdentifier('device1', 'servo'))
@TransceiverFunction("client")
def transceiver_function(device1):
    rec_device1 = PythonDevice("rec_device1", "client")
    rec_device1.data = device1.data
    print("Main: ")
    print(rec_device1.data)
    return [ rec_device1 ]


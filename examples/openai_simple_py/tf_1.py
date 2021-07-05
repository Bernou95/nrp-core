from NRPPythonModule import *
from test import *

@FromEngineDevice(keyword='device1', id=DeviceIdentifier('device1', 'server'))
@TransceiverFunction("client")
def transceiver_function(device1):
	#test_1()
	rec_device1 = PythonDevice("rec_device1", "client")
	rec_device1.data = device1.data
	#print("Transceiver Function: ", rec_device1.data)
	return [ rec_device1 ]


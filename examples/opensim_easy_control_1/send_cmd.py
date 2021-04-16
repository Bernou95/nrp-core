from NRPPythonModule import *

@FromEngineDevice(keyword='action', id=DeviceIdentifier('action', 'client'))
@TransceiverFunction("server")
def transceiver_function(action):
    rec_cmd = PythonDevice("control_cmd", "server")
    rec_cmd.data = action.data

    return [ rec_cmd ]

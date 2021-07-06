from NRPPythonModule import *

@FromEngineDevice(keyword='spikerecorderr', id=DeviceIdentifier('spikerecorderr', 'nest'))
@FromEngineDevice(keyword='spikerecorderl', id=DeviceIdentifier('spikerecorderl', 'nest'))
@TransceiverFunction("gym")
def transceiver_function(spikerecorderr, spikerecorderl):
    rec_action = PythonDevice("action", "gym")

    spikesr = spikerecorderr.data[0]['n_events']
    spikesl = spikerecorderl.data[0]['n_events']

    if spikesr>spikesl:
        rec_action.data = { "action" : 2}
    elif spikesr<spikesl:
        rec_action.data = { "action" : 0}
    else:
        rec_action.data = { "action" : 1}

    return [rec_action]

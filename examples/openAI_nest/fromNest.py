from NRPPythonModule import *

@FromEngineDevice(keyword='spikerecorderr', id=DeviceIdentifier('spikerecorderr', 'nest'))
@FromEngineDevice(keyword='spikerecorderl', id=DeviceIdentifier('spikerecorderl', 'nest'))
@FromEngineDevice(keyword='opFlag', id=DeviceIdentifier('opFlag', 'gym'))
@TransceiverFunction("gym")
def transceiver_function(spikerecorderr, spikerecorderl, opFlag):
    rec_action = PythonDevice("action", "gym")
    rec_op = PythonDevice("opFlag", "gym")

    if opFlag.data['opFlag'] == 0:
        spikesr = spikerecorderr.data[0]['n_events']
        spikesl = spikerecorderl.data[0]['n_events']

        if spikesr>spikesl:
            rec_action.data = { "action" : 2}
        elif spikesr<spikesl:
            rec_action.data = { "action" : 0}
        else:
            rec_action.data = { "action" : 1}

        rec_op.data = { "opFlag" : 1}
        return [rec_action, rec_op]
    else:
        rec_op.data = { "opFlag" : 0}
        return [rec_op]

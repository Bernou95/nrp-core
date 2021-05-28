from NRPPythonModule import *
from NRPNestJSONPythonModule import NestDevice

@FromEngineDevice(keyword='observation', id=DeviceIdentifier('observation', 'gym'))
@FromEngineDevice(keyword='opFlag', id=DeviceIdentifier('opFlag', 'gym'))
@TransceiverFunction("nest")
def transceiver_function(observation, opFlag):
    
    if opFlag.data['opFlag'] == 1:
        dcl = NestDevice("dcl", "nest")
        dcr = NestDevice("dcr", "nest")

        theObsver = observation.data.get('observation')
        dcl.data = {"amplitude": theObsver[1]*10000.0}
        dcr.data = {"amplitude": theObsver[1]*10000.0}

        return [ dcl, dcr ]
    else:
        spikerecorderr = NestDevice("spikerecorderr", "nest")
        spikerecorderl = NestDevice("spikerecorderl", "nest")

        spikerecorderr.data = {"n_events":0}
        spikerecorderl.data = {"n_events":0}
        return [spikerecorderr, spikerecorderl]



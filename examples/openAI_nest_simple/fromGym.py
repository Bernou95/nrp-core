from NRPPythonModule import *
from NRPNestJSONPythonModule import NestDevice

@FromEngineDevice(keyword='observation', id=DeviceIdentifier('observation', 'gym'))
@TransceiverFunction("nest")
def transceiver_function(observation):
    dcl = NestDevice("dcl", "nest")
    dcr = NestDevice("dcr", "nest")

    theObsver = observation.data.get('observation')
    dcl.data = {"amplitude": theObsver[1]*10000.0}
    dcr.data = {"amplitude": theObsver[1]*10000.0}

    spikerecorderr = NestDevice("spikerecorderr", "nest")
    spikerecorderl = NestDevice("spikerecorderl", "nest")

    spikerecorderr.data = {"n_events":0}
    spikerecorderl.data = {"n_events":0}
    return [ dcl, dcr, spikerecorderr, spikerecorderl]


from nrp_core import *
from nrp_core.data.nrp_json import *

@FromEngineDataPack(keyword='observation', id=DataPackIdentifier('observation', 'gym'))
@TransceiverFunction("nest")
def transceiver_function(observation):
    dcl = JsonDataPack("dcl", "nest")
    dcr = JsonDataPack("dcr", "nest")
    
    theObsver = observation.data["observation"]
    dcl.data["amplitude"] = theObsver[1]*10000.0
    dcr.data["amplitude"] = theObsver[1]*10000.0

    spikerecorderr = JsonDataPack("spikerecorderr", "nest")
    spikerecorderl = JsonDataPack("spikerecorderl", "nest")

    spikerecorderr.data["n_events"] = 0
    spikerecorderl.data["n_events"] = 0
    return [ dcl, dcr, spikerecorderr, spikerecorderl]


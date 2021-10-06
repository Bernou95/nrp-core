from nrp_core import *
from nrp_core.data.nrp_json import *

@FromEngineDataPack(keyword='observation', id=DataPackIdentifier('observation', 'gym'))
@TransceiverFunction("nest")
def transceiver_function(observation):
    theObsver = observation.data["observation"]
    dc_list = []
    for i in range(60):
        dc_name = "dc_"+ str(i)
        dc_tmp = JsonDataPack(dc_name, "nest")
        dc_list.append(dc_tmp)
    dc_list[0].data["amplitude"] = theObsver[1]*10000.0
    dc_list[1].data["amplitude"] = theObsver[1]*10000.0
    '''
    theObsver = observation.data["observation"]

    dcl = JsonDataPack("dcl", "nest")
    dcr = JsonDataPack("dcr", "nest")
    dcl.data["amplitude"] = theObsver[1]*10000.0
    dcr.data["amplitude"] = theObsver[1]*10000.0
    '''
    spikerecorderr = JsonDataPack("spikerecorderr", "nest")
    spikerecorderl = JsonDataPack("spikerecorderl", "nest")

    spikerecorderr.data["n_events"] = 0
    spikerecorderl.data["n_events"] = 0
    returnList = []
    returnList.extend(dc_list)
    returnList.append(spikerecorderr, spikerecorder)
    return returnList#[ dcl, dcr, spikerecorderr, spikerecorderl]


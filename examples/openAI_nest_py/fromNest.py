from nrp_core import *
from nrp_core.data.nrp_json import *

@FromEngineDataPack(keyword='spikerecorderr', id=DataPackIdentifier('spikerecorderr', 'nest'))
@FromEngineDataPack(keyword='spikerecorderl', id=DataPackIdentifier('spikerecorderl', 'nest'))
@TransceiverFunction("gym")
def transceiver_function(spikerecorderr, spikerecorderl):
    rec_action = JsonDataPack("action", "gym")

    spikesr = spikerecorderr.data[0]['n_events']
    spikesl = spikerecorderl.data[0]['n_events']

    if spikesr>spikesl:
        rec_action.data["action"] = 2
    elif spikesr<spikesl:
        rec_action.data["action"] = 0
    else:
        rec_action.data["action"] = 1

    return [rec_action]

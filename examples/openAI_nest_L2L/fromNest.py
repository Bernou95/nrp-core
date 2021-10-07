from nrp_core import *
from nrp_core.data.nrp_json import *

@FromEngineDataPack(keyword='spike_detector', id=DataPackIdentifier('spike_detector', 'nest'))
@TransceiverFunction("gym")
def transceiver_function(spike_detector):
    rec_action = JsonDataPack("action", "gym")
    push_left = spike_detector.data[0]['n_events']
    push_none = spike_detector.data[1]['n_events']
    push_right = spike_detector.data[2]['n_events']
    if (push_left > push_right and push_left > push_none):
        rec_action.data["action"] =  0
    elif (push_right > push_left and push_right > push_none):
        rec_action.data["action"] =  2
    else:
        rec_action.data["action"] =  1
        
    return [rec_action]
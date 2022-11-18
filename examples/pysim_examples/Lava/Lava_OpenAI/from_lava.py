from nrp_core import *
from nrp_core.data.nrp_json import *

@EngineDataPack(keyword='output', id=DataPackIdentifier('output', 'lava_controller'))
@TransceiverFunction("gym_simulator")
def transceiver_function(output):
    rec_action = JsonDataPack("action", "gym_simulator")

    signal_data = output.data["output"]

    spikesr = signal_data[0]
    spikesl = signal_data[1]

    if spikesr>spikesl:
        rec_action.data["action"] = 2
    elif spikesr<spikesl:
        rec_action.data["action"] = 0
    else:
        rec_action.data["action"] = 1
    return [rec_action]

from nrp_core import *
from nrp_core.data.nrp_json import *

@EngineDataPack(keyword='observation', id=DataPackIdentifier('observation', 'gym_simulator'))
@TransceiverFunction("lava_controller")
def transceiver_function(observation):
    dc = JsonDataPack("input", "lava_controller")

    the_obsver = observation.data["observation"]
    dc.data['input'] = [the_obsver[1]*10000.0, the_obsver[1]*10000.0]

    return [ dc ]

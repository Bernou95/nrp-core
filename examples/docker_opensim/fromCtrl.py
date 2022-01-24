from nrp_core import *
from nrp_core.data.nrp_json import *

@EngineDataPack(keyword='act_list', id=DataPackIdentifier('act_list', 'python_1'))
@TransceiverFunction("docker_server")
def transceiver_function(act_list):
    rec_datapack1 = JsonDataPack("actionList", "docker_server")
    for k in act_list.data.keys():
        rec_datapack1.data[k] = act_list.data[k]
    return [ rec_datapack1 ]


from nrp_core import *
from nrp_core.data.nrp_json import *

@EngineDataPack(keyword='dNode', id=DataPackIdentifier('dNode', 'docker_server'))
@TransceiverFunction("python_1")
def transceiver_function(dNode):
    rec_datapack1 = JsonDataPack("info", "python_1")
    if dNode.data == None:
        return []
    for k in dNode.data.keys():
        rec_datapack1.data[k] = dNode.data[k]

    return [ rec_datapack1 ]


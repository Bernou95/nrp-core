from nrp_core import *
from nrp_core.data.nrp_json import *
import numpy as np

def encode_values(min_range, max_range, bins, value):
    if max_range == value:
        return bins
    x = np.linspace(min_range, max_range, bins)
    hist, edges = np.histogram(x, bins=bins)
    return np.digitize(value, edges)
old_neuro_list = [0, 0]

@FromEngineDataPack(keyword='observation', id=DataPackIdentifier('observation', 'gym'))
@TransceiverFunction("nest")
def transceiver_function(observation):
    theObserver = observation.data["observation"]

    ap_name = "ap_"+str(old_neuro_list[0])
    ap_reset = JsonDataPack(ap_name, "nest")
    ap_reset.data["amplitude"] = 0.0
    ac_name = "ac_"+str(old_neuro_list[1])
    ac_reset = JsonDataPack(ac_name, "nest")
    ac_reset.data["amplitude"] = 0.0
    
    position = theObserver[0]
    velocity = theObserver[1]
    velocity_neuron = int(encode_values(-0.07, 0.07, 30, velocity)) - 1
    position_neuron = int(encode_values(-1.2, 0.6, 30, position)) - 1
    ac_name = "ac_"+str(velocity_neuron)
    ac_update = JsonDataPack(ac_name, "nest")
    ac_update.data["amplitude"] = 90000.0
    ap_name = "ap_"+str(position_neuron)
    ap_update = JsonDataPack(ap_name, "nest")
    ap_update.data["amplitude"] = 90000.0
    
    spike_detector = JsonDataPack("spike_detector", "nest")
    for i in range(3):
        spike_detector.data.append({"n_events": 0})

    returnList = [ac_update, ap_update, spike_detector]
    if old_neuro_list[0] != position_neuron:
        returnList.append(ap_reset)
    if old_neuro_list[1] != velocity_neuron:
        returnList.append(ac_reset)

    old_neuro_list[0] = position_neuron
    old_neuro_list[1] = velocity_neuron

    return returnList


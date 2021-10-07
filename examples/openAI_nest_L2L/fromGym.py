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
    position = theObserver[0]
    velocity = theObserver[1]
    velocity_neuron = int(encode_values(-0.07, 0.07, 30, velocity)) - 1
    position_neuron = int(encode_values(-1.2, 0.6, 30, position)) - 1

    activator_position = JsonDataPack("activator_position", "nest")
    for i in range(30):
        if i == position_neuron:
            activator_position.data.append({'amplitude': 90000.0})
        elif i == old_neuro_list[0]:
            activator_position.data.append({'amplitude': 0.0})
        else:
            activator_position.data.append({})
    activator_velocity = JsonDataPack("activator_velocity", "nest")
    for i in range(30):
        if i == velocity_neuron:
            activator_velocity.data.append({'amplitude': 90000.0})
        elif i == old_neuro_list[1]:
            activator_velocity.data.append({'amplitude': 0.0})
        else:
            activator_velocity.data.append({})

    spike_detector = JsonDataPack("spike_detector", "nest")
    for i in range(3):
        spike_detector.data.append({"n_events": 0})

    old_neuro_list[0] = position_neuron
    old_neuro_list[1] = velocity_neuron
    return [activator_position, activator_velocity, spike_detector]



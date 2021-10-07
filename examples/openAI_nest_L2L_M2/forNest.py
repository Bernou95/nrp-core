import nest
import nest.voltage_trace
from nrp_core.engines.nest_json import RegisterDataPack, CreateDataPack
import pandas as pd


df = pd.read_csv("individual_config.csv", header=None, na_filter=False)
weights = df.iloc[0].values * 100

nest.ResetKernel()
nest.set_verbosity('M_ERROR')
nest.SetKernelStatus({
    'resolution': 0.2
    #'rng_seed': 1,
    })
# Create Nodes
params = {'t_ref': 1.0}
activator_velocity = []
activator_position = []
for i in range(30):
    # activator_velocity
    ac_name = "ac_"+str(i)
    ac_tmp = CreateDataPack(ac_name, "dc_generator")
    nest.SetStatus(ac_tmp, {'amplitude': 0.})
    activator_velocity.append(ac_tmp)
    # activator_position
    ap_name = "ap_"+str(i)
    ap_tmp = CreateDataPack(ap_name, "dc_generator")
    nest.SetStatus(ap_tmp, {'amplitude': 0.})
    activator_position.append(ap_tmp)

spike_detector = CreateDataPack("spike_detector", "spike_recorder", 3)
for i in range(3):
    nest.SetStatus(spike_detector[i], {'n_events':0})

inpt_velocity = nest.Create('iaf_psc_alpha', 30, params=params)
inpt_position = nest.Create('iaf_psc_alpha', 30, params=params)
nodes = nest.Create('iaf_psc_alpha', 5, params=params)
outpt = nest.Create('iaf_psc_alpha', 3, params=params)
# Network Connect
for i in range(30):
    nest.Connect(activator_velocity[i],inpt_velocity[i], 'one_to_one')
    nest.Connect(activator_position[i],inpt_position[i], 'one_to_one')

syn_spec_dict = {'weight': weights[0:150].reshape(5, 30)}
nest.Connect(inpt_velocity, nodes, 'all_to_all', syn_spec=syn_spec_dict)
syn_spec_dict = {'weight': weights[150:300].reshape(5, 30)}
nest.Connect(inpt_position, nodes, 'all_to_all', syn_spec=syn_spec_dict)
syn_spec_dict = {'weight': weights[300:315].reshape(3, 5)}
nest.Connect(nodes, outpt, 'all_to_all', syn_spec=syn_spec_dict)
nest.Connect(outpt, spike_detector, 'one_to_one')

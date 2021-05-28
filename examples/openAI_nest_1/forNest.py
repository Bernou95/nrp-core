import nest
from NRPNestJSONPythonModule import RegisterDevice, CreateDevice

nest.set_verbosity('M_WARNING')
nest.ResetKernel()

neurons = nest.Create('iaf_psc_alpha',3)

dcl = CreateDevice("dcl", "dc_generator")
dcr = CreateDevice("dcr", "dc_generator")
nest.SetStatus(dcl, {"amplitude": 0.0})
nest.SetStatus(dcr, {"amplitude": 0.0})

nest.Connect(dcl, neurons[0], 'all_to_all',
                    {'weight': -500., 'delay': 1.0})
nest.Connect(dcr, neurons[1], 'all_to_all',
                    {'weight': 500., 'delay': 1.0})
spikerecorderr = nest.Create("spike_recorder")
spikerecorderl = nest.Create("spike_recorder")
RegisterDevice('spikerecorderr', spikerecorderr)
RegisterDevice('spikerecorderl', spikerecorderl)

nest.Connect(neurons[1], spikerecorderr, 'all_to_all')
nest.Connect(neurons[0], spikerecorderl, 'all_to_all')




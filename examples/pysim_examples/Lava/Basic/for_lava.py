from nrp_core.engines.py_sim import PyNetEngineScript
import numpy as np


class Script(PyNetEngineScript):
    def initialize(self):
        params_1 = {"shape":(3, ), "vth":10., "dv":0.1, "du":0.1,
            "bias_mant":(1.1, 1.2, 1.3),  "name":"lif1"}
        self.net_manager.create_layer(layer_name='lif1', 
            layer_type='LIF', params=params_1)
        params_2 = {"shape":(2, ), "vth":10., "dv":0.1, "du":0.1,
            "bias_mant":0,  "name":"lif2"}
        self.net_manager.create_layer(layer_name='lif2', 
            layer_type='LIF', params=params_2)

        params_3 = {"weights":np.random.rand(2, 3),  "name":"dense"}
        self.net_manager.create_layer(layer_name='dense', layer_type='Dense', params=params_3)
        
        self.net_manager.layer_connect("lif1.s_out","dense.s_in")
        self.net_manager.layer_connect("dense.a_out","lif2.a_in")
        self.net_manager.create_monitor('lif2.v')
        self.last_layer = "lif2"


    def runLoop(self, timestep_ns):
        self.net_manager.run_step(self.last_layer, timestep_ns)
        var = self.net_manager.getDataFromLayer('lif2','v')

    def shutdown(self):
        self.net_manager.res_plot('lif2','v')
        self.net_manager.shutdown(self.last_layer)



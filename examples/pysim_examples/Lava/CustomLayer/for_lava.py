from nrp_core.engines.py_sim import PyNetEngineScript
import numpy as np


class Script(PyNetEngineScript):
    def initialize(self):
        dim = 400
        shape = (dim,)
        num_neurons_exc = int(dim * 0.8)
        num_neurons_inh = dim - num_neurons_exc

        params_exc = { "shape_exc": num_neurons_exc, "dr_exc": 0.01, "bias_exc": 0.1}
        params_inh = { "shape_inh": num_neurons_inh, "dr_inh": 0.01, "bias_inh": 0.1}
        g_factor = 4.5
        q_factor = 1

        network_params_balanced = {}
        network_params_balanced.update(params_exc)
        network_params_balanced.update(params_inh)
        network_params_balanced['g_factor'] = g_factor
        network_params_balanced['q_factor'] = q_factor

        network_params_balanced['weights'] = self.generate_gaussian_weights(dim, num_neurons_exc,
            network_params_balanced['q_factor'], network_params_balanced['g_factor'])

        self.net_manager.create_layer(laver_name='network_balanced', 
            layer_type='EINetwork', params=network_params_balanced)

        self.net_manager.create_monitor('network_balanced.state')
        self.last_layer = "network_balanced"


    def runLoop(self, timestep_ns):
        self.net_manager.run_step(self.last_layer, timestep_ns)

    def shutdown(self):
        self.net_manager.res_plot('network_balanced','state')
        self.net_manager.shutdown(self.last_layer)

    def generate_gaussian_weights(self, dim, num_neurons_exc, q_factor, g_factor):
        J = (2 * q_factor)**2 / dim
        weights = np.random.normal(0, J, (dim, dim))

        exc_conns = np.full(weights.shape, True)
        exc_conns[:, num_neurons_exc:] = False 
        inh_conns = np.invert(exc_conns)

        mask_pos_weights = (weights > 0)
        mask_neg_weights = (weights < 0)

        weights[mask_neg_weights * exc_conns] = 0
        weights[mask_pos_weights * inh_conns] = 0

        weights[inh_conns] *= g_factor

        return weights

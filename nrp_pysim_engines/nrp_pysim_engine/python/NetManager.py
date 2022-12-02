import time
import importlib
import numpy as np

from lava.magma.core.run_conditions import RunSteps, RunContinuous
from lava.magma.core.run_configs import RunConfig, Loihi2SimCfg
from lava.proc.monitor.process import Monitor
from matplotlib import pyplot as plt

class NetManager(object):

    def __init__(self, configuration):
        self.time_step = configuration["EngineTimestep"]
        self.run_cfg = Loihi2SimCfg(select_tag=configuration["RunConfigureTag"])
        self.layer_list = {}
        self.modules_list = {}
        self.monitor_list = {}

        for package_name, module_name in configuration["Layers"]:
            the_package = importlib.import_module(package_name)
            self.modules_list[module_name] = getattr(the_package, module_name)

    def shutdown(self, layer_name):
        self.layer_list[layer_name].stop()

    def run_step(self, layer_name, timestep_ns):
        run_condition = RunSteps(num_steps=1e8/timestep_ns)
        self.layer_list[layer_name].run(condition=run_condition, run_cfg=self.run_cfg)

        for key in self.monitor_list.keys():
            t_layer_name, t_para_name = key.split('.')
            self.monitor_list[key].append(
                getattr(self.layer_list[t_layer_name], t_para_name).get())

    def create_layer(self, layer_name, layer_type, params):
        self.layer_list[layer_name] = self.modules_list[layer_type](**params)

    def create_monitor(self, para_name):
        self.monitor_list[para_name] = []

    def res_plot(self, layer_name, para_name):
        var = self.monitor_list[layer_name+'.'+para_name]
        var_num = len(var[0])
        for i in range(var_num):
            plt.plot([data[i] for data in var])
        plt.show()

    def get_port(self, port_name):
        the_port = port_name.split('.')
        return getattr(self.layer_list[the_port[0]], the_port[1])

    def layer_connect(self, in_port, out_port):
        t_in_port = self.get_port(in_port)
        t_out_port = self.get_port(out_port)
        t_in_port.connect(t_out_port)

    def setDataToLayer(self, layer_name, para_name, data):
        var = getattr(self.layer_list[layer_name], para_name)
        var.set(np.array(data))
        
    def getDataFromLayer(self, layer_name, para_name):
        return getattr(self.layer_list[layer_name], para_name).get()


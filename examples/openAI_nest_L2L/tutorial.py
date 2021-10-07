import argparse
import gym
import nest
import numpy as np
import pandas as pd



class NestNetwork():
    def __init__(self):
        nest.ResetKernel()
        nest.set_verbosity('M_ERROR')
        nest.SetKernelStatus(
            {
                'resolution': 0.2
        #        'rng_seed': 1,
            })
        self.inpt_velocity = None
        self.inpt_position = None
        self.activator_velocity = None
        self.activator_position = None
        self.outpt = None
        self.nodes = None
        self.spike_detector = None

        self.create_nodes()

    def connect_synapses(self):
        # Connect synapses
        nest.CopyModel('static_synapse', 'random_synapse')  # syn_spec=syn_spec)


    def create_nodes(self):
        params = {'t_ref': 1.0}
        #self.activator_velocity = nest.Create("dc_generator", 30, params={'amplitude': 0.})
        #self.activator_position = nest.Create("dc_generator", 30, params={'amplitude': 0.})
        self.activator_velocity = []
        self.activator_position = []
        for i in range(30):
            tV = nest.Create("dc_generator")
            nest.SetStatus(tV, [{"amplitude": 0}])
            self.activator_velocity.append(tV)
            tP = nest.Create("dc_generator")
            nest.SetStatus(tV, [{"amplitude": 0}])
            self.activator_position.append(tP)

        self.inpt_velocity = nest.Create('iaf_psc_alpha', 30, params=params)
        self.inpt_position = nest.Create('iaf_psc_alpha', 30, params=params)
        self.nodes = nest.Create('iaf_psc_alpha', 5, params=params)
        self.outpt = nest.Create('iaf_psc_alpha', 3, params=params)
        self.spike_detector = nest.Create('spike_recorder', 3)


    def connect_network(self,weights):
        for i in range(30):
            nest.Connect(self.activator_velocity[i], self.inpt_velocity[i], 'one_to_one')
            nest.Connect(self.activator_position[i], self.inpt_position[i], 'one_to_one')
        #nest.Connect(self.activator_velocity, self.inpt_velocity, 'one_to_one')
        #nest.Connect(self.activator_position, self.inpt_position, 'one_to_one')
        syn_spec_dict = {'weight': weights[0:150].reshape(5, 30)}
        nest.Connect(self.inpt_velocity, self.nodes, 'all_to_all', syn_spec=syn_spec_dict)
        syn_spec_dict = {'weight': weights[150:300].reshape(5, 30)}
        nest.Connect(self.inpt_position, self.nodes, 'all_to_all', syn_spec=syn_spec_dict)
        syn_spec_dict = {'weight': weights[300:315].reshape(3, 5)}
        nest.Connect(self.nodes, self.outpt, 'all_to_all', syn_spec=syn_spec_dict)
        nest.Connect(self.outpt, self.spike_detector, 'one_to_one')

    def simulate(self, sim_time=20.0):
        nest.Simulate(sim_time)

    def outStatus(self):
        tList = []
        for i in range(30):
            tVal = nest.GetStatus(self.activator_velocity[i], 'amplitude')
            if tVal[0] != 0.0:
                print(i, " --> ", tVal[0])
            tList.append(tVal[0])
        print(tList)
    def feed_network(self, velocity_neuron, position_neuron):
        nest.SetStatus(self.activator_velocity[velocity_neuron], [{'amplitude':90000.0}])
        nest.SetStatus(self.activator_position[position_neuron], [{'amplitude':90000.0}])

    def setZeroNeuron(self, velocity_neuron, position_neuron):
        nest.SetStatus(self.activator_velocity[velocity_neuron], [{'amplitude':0.0}])
        nest.SetStatus(self.activator_position[position_neuron], [{'amplitude':0.0}])
        self.spike_detector.set({'n_events':0})

    def get_action_from_network(self):
        push_left = nest.GetStatus(self.spike_detector[0], 'n_events')
        push_none = nest.GetStatus(self.spike_detector[1], 'n_events')
        push_right = nest.GetStatus(self.spike_detector[2], 'n_events')
        if (push_left > push_right and push_left > push_none):
            return 0
        elif (push_right > push_left and push_right > push_none):
            return 2
        else:
            return 1

    def set_gym_action(self, action, env):
        observation, reward, done, info = env.step(action)
        env.render()
        return observation


def encode_values(min_range, max_range, bins, value):
    if max_range == value:
        return bins
    x = np.linspace(min_range, max_range, bins)
    hist, edges = np.histogram(x, bins=bins)
    return np.digitize(value, edges)

def test_network(network, env,old_neuro_list):
    observation = env.reset()
    position = observation[0]
    velocity = observation[1]
    max_position = -2.0
    fitness = 0
    print(old_neuro_list)
    for sim_step in range(1, 201):
        #network.simulate(30.)
        #print("--------------------------------------------------------")
        #network.outStatus()
        network.setZeroNeuron(old_neuro_list[0], old_neuro_list[1])
        #network.reset_activators_and_recorders()
        velocity_neuron = int(encode_values(-0.07, 0.07, 30, velocity)) - 1
        position_neuron = int(encode_values(-1.2, 0.6, 30, position)) - 1
        network.feed_network(velocity_neuron, position_neuron)
        old_neuro_list[0] = velocity_neuron
        old_neuro_list[1] = position_neuron
        #network.outStatus()
        network.simulate(20.)
        action = network.get_action_from_network()
        observation = network.set_gym_action(action, env)
        position = observation[0]
        velocity = observation[1]
        if max_position < position:
            max_position = position

        if position >= 0.5:
            #print(sim_step)
            break
    #network.setZeroNeuron(old_vn, old_pn)
    print(sim_step, max_position)
    return sim_step

if __name__ == '__main__':
    df = pd.read_csv("individual_config.csv", header=None, na_filter=False)
    weights = df.iloc[0].values * 100
    network = NestNetwork()
    network.connect_network(weights)
    env = gym.make('MountainCar-v0')
    sum_fitness = 0
    old_neuro_list = [0, 0]
    for i in range(0, 6):
    	sum_fitness = sum_fitness + test_network(network, env,old_neuro_list)
    print("Mean fitness: ", sum_fitness/5)	
    	


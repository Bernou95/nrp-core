import nest
import gym
import numpy as np
def get_action(observation):
   nest.SetStatus(dcr, [{"amplitude": observation[1]*10000}])
   nest.SetStatus(dcl, [{"amplitude": observation[1]*10000}])
   #print(observation)
   nest.Simulate(1000)
   spikesr = nest.GetStatus(spikerecorderr)[0]['n_events']
   spikesl = nest.GetStatus(spikerecorderl)[0]['n_events']
   #print(spikesr)
   #print(spikesl)
   nest.SetStatus(spikerecorderr,{"n_events":0})
   nest.SetStatus(spikerecorderl,{"n_events":0})
   if spikesr>spikesl:
       action = 2
   elif spikesr<spikesl:
       action = 0
   else:
       action = 1
   #print(action)
   return action

   
nest.ResetKernel()
nest.set_verbosity('M_ERROR')
nest.SetKernelStatus(
           {
               'resolution': 1.0
           })
neurons = nest.Create('iaf_psc_alpha',3)
dcl = nest.Create("dc_generator")
dcr = nest.Create("dc_generator")
#print(neurons)
nest.Connect(dcl, neurons[0], 'all_to_all',
                    {'weight': -500., 'delay': 1.0})
nest.Connect(dcr, neurons[1], 'all_to_all',
                    {'weight': 500., 'delay': 1.0})
spikerecorderr = nest.Create("spike_recorder")
spikerecorderl = nest.Create("spike_recorder")
nest.Connect(neurons[1], spikerecorderr, 'all_to_all')
nest.Connect(neurons[0], spikerecorderl, 'all_to_all')
env = gym.make('MountainCar-v0')
for i_episode in range(2):
   observation = env.reset()
   print(observation)
   for t in range(201):
       env.render()
       #print(observation)
       action = get_action(observation)
       #print(action)
       observation, reward, done, info = env.step(action)
       if done:
           print("Episode finished after {} timesteps".format(t+1))
           break
env.close()
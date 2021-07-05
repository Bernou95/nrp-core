import gym
from gym import wrappers
from gym import spaces
import numpy as np

class TGymLib(object):
	"""docstring for TGymLib"""
	action = None

	reward = None
	observation = None
	doneFlag = None
	info = None

	
	def __init__(self, isVisualizer, modelName, theWrappers="", selfEnv=None):
		super(TGymLib, self).__init__()
		if selfEnv != None:
			self.env = selfEnv
		else:
			self.env = gym.make(modelName)

		if theWrappers != "":
			self.env = wrappers.Monitor(self.env, theWrappers, force=True)
		
		self.observation = self.env.reset()
		self.isVisualizer = isVisualizer

	
	def getAction(self):
		return self.action
	def getState(self):
		return self.env.state

	def getReward(self):
		return self.reward
	def getObservation(self):
<<<<<<< HEAD
		return self.observation
	def getDone(self):
		return self.doneFlag
	def getTrainInfo(self):
		return self.info

	def run_action_step(self, theAction):
		if self.isVisualizer:
			self.env.render()
		tObservation, tReward, tDoneFlag, tInfo = self.env.step(theAction)
		self.observation = tObservation
		self.reward = tReward
		self.doneFlag = tDoneFlag
		self.info = tInfo
		if self.doneFlag: 
			self.observation = self.env.reset()
		return self.doneFlag

=======
		return observation
	def getDone(self):
		return doneFlag
	def getTrainInfo(self):
		return self.info

>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
	def run_step(self):
		if self.isVisualizer:
			self.env.render()
		self.action = self.env.action_space.sample()
		tObservation, tReward, tDoneFlag, tInfo = self.env.step(self.action)
<<<<<<< HEAD

		self.observation = tObservation
=======
		self.tObservation = tObservation
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
		self.reward = tReward
		self.doneFlag = tDoneFlag
		self.info = tInfo
		if self.doneFlag: 
			self.observation = self.env.reset()
		return self.doneFlag

	def reset(self):
		self.observation = self.env.reset()
		return self.observation

	def shutdown(self):
		self.observation = self.env.reset()
		self.env.close()

class OwnAgent(gym.Env):
	"""docstring for OwnAgent"""
	from gym import spaces
	def __init__(self):
		super(OwnAgent, self).__init__()
		self.action_space = None
		self.observation_space = None
		self.state = None
		pass

	def step(self, action):
		return self.state, reward, done, {}

	def reset(self):
		return self.state

	def render(self, mode='human'):
		return None
	
	def close(self):
		return None


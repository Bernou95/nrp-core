from mujoco_py import load_model_from_path, MjSim, MjViewer
import math

class TMujocoLib(object):
	"""docstring for TMujocoLib"""
	def __init__(self, modelPath, isVisualizer):
		super(TMujocoLib, self).__init__()
		self.model = load_model_from_path(modelPath)
		self.sim = MjSim(self.model)

		# Mini timestep of Mujoco is 2ms
		self.stepsize = self.model.opt.timestep

		self.sim_state = self.sim.get_state()
		
		self.isVisualizer = isVisualizer
		if isVisualizer:
			self.viewer = MjViewer(self.sim)

	def getSimData(self):
		return self.sim.data

	def run_step(self, action, timeStep_ns):
		timeStep_s = timeStep_ns/math.pow(10, 9)
		numIterations = int(timeStep_s/self.stepsize)

		self.sim.data.ctrl[:] = action
		for i in range(numIterations):
			self.sim.step()
		if self.isVisualizer:
			self.viewer.render()
			
	def reset(self):
		self.sim.reset()

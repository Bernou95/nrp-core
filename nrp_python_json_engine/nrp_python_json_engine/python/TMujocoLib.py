from mujoco_py import load_model_from_path, MjSim, MjViewer

class TMujocoLib(object):
	"""docstring for TMujocoLib"""
	def __init__(self, modelPath, isVisualizer):
		super(TMujocoLib, self).__init__()
		self.model = load_model_from_path(modelPath)
		self.sim = MjSim(self.model)
		

		self.sim_state = self.sim.get_state()
		
		self.isVisualizer = isVisualizer
		if isVisualizer:
			self.viewer = MjViewer(self.sim)

	def getSimData(self):
		return self.sim.data

	def run_step(self, action):
		self.sim.data.ctrl[:] = action
		self.sim.step()
		if self.isVisualizer:
			self.viewer.render()
			
	def reset(self):
		self.sim.reset()

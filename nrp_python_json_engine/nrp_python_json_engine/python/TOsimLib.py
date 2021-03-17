import opensim as osim
class TOpenSim(object):
	stepsize = 0.001

	model = None
	state = None
	state0 = None
	joints = []
	bodies = []
	brain = None
	verbose = False
	istep = 0

	state_desc_istep = None
	prev_state_desc = None
	state_desc = None
	integrator_accuracy = 5e-5

	maxforces = []
	curforces = []
	"""docstring for t"""
	def __init__(self, modelName, isVisualizer):		
		super(TOpenSim, self).__init__()
		self.model = osim.Model(modelName)
		self.model.initSystem()
		self.model.setUseVisualizer(isVisualizer)
		self.brain = osim.PrescribedController()
		self.muscleSet = self.model.getMuscles()
		for j in range(self.muscleSet.getSize()):
			func = osim.Constant(1.0)
			self.brain.addActuator(self.muscleSet.get(j))
			self.brain.prescribeControlForActuator(j, func)

			self.maxforces.append(self.muscleSet.get(j).getMaxIsometricForce())
			self.curforces.append(1.0)

		self.noutput = self.muscleSet.getSize()

		self.model.addController(self.brain)
		self.model.initSystem()
		# Enable the visualizer
		self.reset()

	def testPrint(self):
		print("Hello, I am TOpenSim")

	def run_step(self, action):
		self.actuate(action)
		self.integrate()

	def integrate(self):
		# Define the new endtime of the simulation
		self.istep = self.istep + 1
		# Integrate till the new endtime
		try:
			self.state = self.manager.integrate(self.stepsize * self.istep)
		except Exception as e:
			print (e)

	def actuate(self, action):
		# TODO: Check if actions within [0,1]
		self.last_action = action

		brain = osim.PrescribedController.safeDownCast(
			self.model.getControllerSet().get(0))
		functionSet = brain.get_ControlFunctions()

		for j in range(functionSet.getSize()):
			func = osim.Constant.safeDownCast(functionSet.get(j))
			func.setValue(float(action[j]))


	def reset_manager(self):
		self.manager = osim.Manager(self.model)
		self.manager.setIntegratorAccuracy(self.integrator_accuracy)
		self.manager.initialize(self.state)

	def reset(self):
		self.state = self.model.initializeState()
		self.state.setTime(0)
		self.istep = 0

		self.reset_manager()

"""Python Engine 1. Will get current engine time and make it accessible as a device"""

from NRPOpensimEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    import opensim as osim
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

    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.modelFileName = self._world
        
        self.model = self.osim.Model(self.modelFileName)
        self.model.initSystem()
        self.brain = self.osim.PrescribedController()
        self.model.setUseVisualizer(True)
        self.muscleSet = self.model.getMuscles()
        for j in range(self.muscleSet.getSize()):
            func = self.osim.Constant(1.0)
            self.brain.addActuator(self.muscleSet.get(j))
            self.brain.prescribeControlForActuator(j, func)

            self.maxforces.append(self.muscleSet.get(j).getMaxIsometricForce())
            self.curforces.append(1.0)

        self.noutput = self.muscleSet.getSize()

        self.model.addController(self.brain)
        self.model.initSystem()
        # Enable the visualizer
        self.reset()

        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        #'''
        self.actuate([0.0, 0.0, 0.0, 0.5, 0.0, 0.0])
        #: Integration musculoskeletal system
        self.integrate()
        #: Results from the integration
        #print(self.state.getTime())

        self._setDevice("device1", { "cmd" : 1, "time": self.state.getTime() })
        print("Servo data is " + str(self._getDevice("device1")))

    def shutdown():
        print("Engine 1 is shutting down")


    def reset_manager(self):
        self.manager = self.osim.Manager(self.model)
        self.manager.setIntegratorAccuracy(self.integrator_accuracy)
        self.manager.initialize(self.state)

    def reset(self):
        self.state = self.model.initializeState()
        self.state.setTime(0)
        self.istep = 0

        self.reset_manager()

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

        brain = self.osim.PrescribedController.safeDownCast(
            self.model.getControllerSet().get(0))
        functionSet = brain.get_ControlFunctions()

        for j in range(functionSet.getSize()):
            func = self.osim.Constant.safeDownCast(functionSet.get(j))
            func.setValue(float(action[j]))

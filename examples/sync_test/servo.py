"""Python Engine 1. Will get current engine time and make it accessible as a device"""
from NRPOpensimEngineModule import EngineScript,RegisterEngine
from NRPOpensimEngineModule import TOpenSim

@RegisterEngine()
class Script(EngineScript):
    '''
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
    '''
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.modelFileName = self._world

        self.tOS = TOpenSim(self.modelFileName)
        self.tt = SimulationTime()
        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        self.tOS.run_step([0.0, 0.0, 0.0, 0.5, 0.0, 0.0])
        self._setDevice("device1", { "cmd" : 1, "time": self.tOS.state.getTime() })
        self.tt = self._time
        print(self.tt.count())
        print("Servo data is " + str(self._getDevice("device1")))

    def shutdown(self):
        print("Engine 1 is shutting down")



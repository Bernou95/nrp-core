"""Python Engine 1. Will get current engine time and make it accessible as a device"""
from NRPPythonEngineModule import EngineScript,RegisterEngine
from NRPPythonEngineModule import TGymLib

@RegisterEngine()
class Script(EngineScript):
    import math
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.tGym = TGymLib(True, "CartPole-v1")
        #self.tGym = TGymLib(True, 'MsPacman-v0', '/tmp/atari-experiment-2')
        #self.tGym = TGymLib(True, 'Hopper-v2', '/tmp/hopper-experiment-1')
        
        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })
        self.endFlag = False
        self.stepCount = 0

    def runLoop(self, timestep) :
        """Update device1 at every timestep"""
        #from test import test_1 
        #test_1()
        self.endFlag = self.tGym.run_step()
        self.stepCount = self.stepCount + 1
        if self.endFlag:
            #print("Episode finished after {} timesteps".format(self.stepCount))
            self.endFlag = False
            self.stepCount = 0

    def shutdown(self):
        self.tGym.shutdown()



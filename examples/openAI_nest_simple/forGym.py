
from NRPPythonEngineModule import EngineScript,RegisterEngine
from NRPPythonEngineModule import TGymLib

@RegisterEngine()
class Script(EngineScript):
    import math
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.tGym = TGymLib(True, "MountainCar-v0")
        
        self._registerDevice("observation")
        self._setDevice("observation", { "observation" : [0.0]*2})
        self.endFlag = False
        self.stepCount = 0

        self._registerDevice("action")
        self._setDevice("action", { "action" : 1})


    def runLoop(self, timestep) :
        """Update device1 at every timestep"""
        #from test import test_1 

        theAction = self._getDevice("action").get('action')
        self.endFlag = self.tGym.run_action_step(theAction)
        self.stepCount = self.stepCount + 1
        tData = list(self.tGym.getObservation())
        self._setDevice("observation", { "observation" : tData})
        #self._setDevice("opFlag", { "opFlag" : 1-theOperation})
        if self.endFlag:
            self.endFlag = False
            self.stepCount = 0

    def shutdown(self):
        self.tGym.shutdown()



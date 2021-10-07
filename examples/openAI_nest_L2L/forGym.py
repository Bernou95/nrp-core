
from nrp_core.engines.python_json import EngineScript,RegisterEngine
from nrp_core.engines.python_json import TGymLib

@RegisterEngine()
class Script(EngineScript):
    import math
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.tGym = TGymLib(True, "MountainCar-v0")
        
        self._registerDataPack("observation")
        self._setDataPack("observation", { "observation" : [0.0]*2})
        self.endFlag = False
        self.stepCount = 0

        self._registerDataPack("action")
        self.maxP = -1

    def runLoop(self, timestep) :
        """Update device1 at every timestep"""
        #from test import test_1 
        theAction = self._getDataPack("action").get('action')
        self.endFlag = self.tGym.run_action_step(theAction)
        self.stepCount = self.stepCount + 1
        tData = self.tGym.getObservation()
        self._setDataPack("observation", { "observation" : tData})
        if tData[0] > self.maxP:
            self.maxP=tData[0]
            #print(self.maxP)
        if tData[0] >= 0.48:
            print(self.stepCount, " -- ", self.maxP)
            self.maxP = 0
            self.stepCount = 0
            self.tGym.env.reset()
            tData = self.tGym.getObservation()
        self._setDataPack("observation", { "observation" : tData})

    def shutdown(self):
        self.tGym.shutdown()



"""Python Engine 1. Will get current engine time and make it accessible as a device"""
from NRPOpensimEngineModule import EngineScript,RegisterEngine
from NRPOpensimEngineModule import TOpenSim

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.modelFileName = self._world

        self.tOS = TOpenSim(self.modelFileName, False)

        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        self.tOS.run_step([0.0, 0.0, 0.0, 0.5, 0.0, 0.0])
        self._setDevice("device1", { "cmd" : 1, "time": self.tOS.state.getTime() })
        print("Servo data is " + str(self._getDevice("device1")))

    def shutdown(self):
        print("Engine 1 is shutting down")



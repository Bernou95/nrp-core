"""Python Engine 1. Will get current engine time and make it accessible as a device"""
from NRPPythonEngineModule import EngineScript,RegisterEngine
from NRPPythonEngineModule import TOpenSim

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.tOS = TOpenSim("arm26/arm26_ground_offset.osim", False)

        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        action = [0.0, 0.0, 0.0, 0.5, 0.0, 0.0]
        self.tOS.run_step(action)
        self._setDevice("device1", { "cmd" : 1, "time": self.tOS.state.getTime() })
        print("Servo data is " + str(self._getDevice("device1")))
        print("--------------------------------------------------")

    def shutdown(self):
        print("Engine 1 is shutting down")



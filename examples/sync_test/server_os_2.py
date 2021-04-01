"""Python Engine 1. Will get current engine time and make it accessible as a device"""
from NRPPythonEngineModule import EngineScript,RegisterEngine
from NRPPythonEngineModule import TOpenSim

@RegisterEngine()
class Script(EngineScript):
    import time
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.tOS = TOpenSim("rajagopal/rajagopal_2015.osim", False)

        self._registerDevice("device_os_2")
        self._setDevice("device_os_2", { "count" : 0, "time": 0 })

        self.count = 0
        self.time.sleep(1)

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        action = []
        self.tOS.run_step(action, timestep.count())
        self._setDevice("device_os_2", {"count":self.count, "time":self.tOS.state.getTime()})
        self.count = self.count + 1

    def shutdown(self):
        print("Engine 1 is shutting down")



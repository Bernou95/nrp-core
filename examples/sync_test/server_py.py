"""Python Engine 1. Will get current engine time and make it accessible as a device"""

from NRPPythonEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    import math
    import time
    def initialize(self):
        """Initialize device1 with time"""
        print("Engine 1 is initializing. Registering device...")
        self._registerDevice("device_py")

        self._setDevice("device_py", {"count" : 0,"time":0.0})
        self.count = 0
        self.time.sleep(1)

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        t_Time = self._time.count() / self.math.pow(10, 9)
        self._setDevice("device_py", { "count":self.count, "time":t_Time})
        self.count = self.count + 1
        
    def shutdown(self):
        print("Engine 1 is shutting down")

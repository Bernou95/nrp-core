"""Python Engine 1. Will get current engine time and make it accessible as a device"""

from NRPPythonEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with time"""
        self.count = 0

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
<<<<<<< HEAD:examples/sync_test/server_py_1.py
=======
        t_Time = self._time.count() / self.math.pow(10, 9)
        self._setDevice("device_py", { "count":self.count, "time":t_Time})
>>>>>>> dc7adbacd5ccbb81923578be0a04c242f0c3a763:examples/sync_test/server_py.py
        self.count = self.count + 1
        
    def shutdown(self):
        self.count = 0

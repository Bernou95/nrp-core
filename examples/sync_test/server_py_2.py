"""Python Engine 1. Will get current engine time and make it accessible as a device"""

from NRPPythonEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with time"""
        self.count = 0

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        self.count = self.count + 1
        
    def shutdown(self):
        self.count = 0

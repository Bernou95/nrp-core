"""Python Engine 1. Will get current engine time and make it accessible as a device"""

from NRPOpensimEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        self._setDevice("device1", { "cmd" : 1, "time": 10 })
        print("Device 1 data is " + str(self._getDevice("device1")))

    def shutdown():
        print("Engine 1 is shutting down")

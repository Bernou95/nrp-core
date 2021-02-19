"""Python Engine 2. Will get time from Engine 1 and print it"""

from NRPOpensimEngineModule import EngineScript,RegisterEngine


@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with empty time"""
        print("Opensim Engine is initializing. Registering device...")
        self._registerDevice("sim_device")

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        dev1 = self._getDevice("sim_device")
        print("Engine Opensim received data is " + str(dev1))

    def shutdown():
        print("Engine Opensim is shutting down")

"""Python Engine 2. Will get time from Engine 1 and print it"""

from NRPPythonEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with empty time"""
        print("Engine 2 is initializing. Registering device...")
        self._registerDevice("rec_os_1")
        self._setDevice("rec_os_1", {"count" : 0,"time":0.0})
        self._registerDevice("rec_os_2")
        self._setDevice("rec_os_2", {"count" : 0,"time":0.0})
        self._registerDevice("rec_py")
        self._setDevice("rec_py", {"count" : 0,"time":0.0})

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        dev1 = self._getDevice("rec_os_1")
        dev2 = self._getDevice("rec_os_2")
        dev3 = self._getDevice("rec_py")
        #print("Engine 2 received data is " + str(dev1))

    def shutdown(self):
        print("Engine 2 is shutting down")

"""Python Engine 2. Will get time from Engine 1 and print it"""

from NRPPythonEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with empty time"""
        print("Engine 2 is initializing. Registering device...")
        self._registerDevice("rec_gz")
        self._setDevice("rec_gz", {"count" : 0,"time":0.0})
        self._registerDevice("rec_os")
        self._setDevice("rec_os", {"count" : 0,"time":0.0})
        self._registerDevice("rec_py")
        self._setDevice("rec_py", {"count" : 0,"time":0.0})

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        dev1 = self._getDevice("rec_gz")
        dev2 = self._getDevice("rec_os")
        dev3 = self._getDevice("rec_py")
        '''
        print("Main: --------------> ")
        print("Gazebo: " + str(dev1))
        print("Opensim: " + str(dev2))
        print("Python: " + str(dev3))
        '''
        #print("Engine 2 received data is " + str(dev1))

    def shutdown(self):
        print("Engine 2 is shutting down")

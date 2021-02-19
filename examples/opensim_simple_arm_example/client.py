"""Python Engine 2. Will get time from Engine 1 and print it"""

from NRPOpensimEngineModule import EngineScript,RegisterEngine


@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        
        """Initialize device1 with empty time"""
        print("Client Engine is initializing. Registering device...")
        self._registerDevice("rec_device1")
        self._setDevice("rec_device1", { "cmd" : 0, "time": 0 })

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        #'''
        dev1 = self._getDevice("rec_device1")
        
        print("Client received data is " + str(dev1))

    def shutdown():
        print("Engine 2 is shutting down")

    


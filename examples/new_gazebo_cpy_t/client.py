"""Python Engine 2. Will get time from Engine 1 and print it"""

from NRPNewGazeboEngineModule import EngineScript,RegisterEngine


@RegisterEngine()
class Script(EngineScript):
    import importlib
    import math
    import time
    

    def initialize(self):
        """Initialize device1 with empty time"""
        print("Client Engine is initializing. Registering device...")
        self._registerDevice("device2")
        self._setDevice("device2", {"time": -1 })

        self._registerDevice("rec_device1")
        self._setDevice("rec_device1", { "cmd" : 0, "time": 0 })
        self.time.sleep(1)

        import tClient
        self.tGz = tClient.GZ_Client()
        self.tGz.setListener('/gazebo/default/world_stats', 'none')

        self.simTime = 0

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        #'''
        self.tGz.toListen()
        self.simTime = self.tGz._sim_time
        dev1 = self._getDevice("rec_device1")
        self._setDevice("device2", { "cmd" : 1, "time": self.simTime })

        print("Client received data is " + str(dev1))

    def shutdown(self):
        self.tGz.shutdown()
        #print("Engine 2 is shutting down")


    


"""Python Engine 1. Will get current engine time and make it accessible as a device"""

from NRPNewGazeboEngineModule import EngineScript,RegisterEngine


@RegisterEngine()
class Script(EngineScript):
    import importlib
    import time
    
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.t_os = self.importlib.import_module('os')
        self.modelFileName = self._world

        self.topicName = "/gazebo/default/world_stats"

        self.simTime = 0
        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })

        self._registerDevice("rec_device2")
        self._setDevice("rec_device2", { "time": 0 })

        import tServer
        self.tGz = tServer.GZ_Server()
        self.tGz.setWorld(self.modelFileName)
        self.tGz.loadWorld('camera_check.sdf')
        self.time.sleep(1)

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        self._setDevice("device1", { "cmd" : 1, "time": 1 })
        dev2 = self._getDevice("rec_device2")
        print("Servo received data is " + str(dev2))


    def shutdown():
        self.tGz.shutdown()
        print("Engine 1 is shutting down")
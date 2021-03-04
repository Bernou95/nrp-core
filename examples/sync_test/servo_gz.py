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

        import tClient
        self.tGz = tClient.GZ_Client()
        self.tGz.setListener('/gazebo/default/world_stats', 'none')

        self.t_os.system("gzserver %s &" % self.modelFileName)

        self.simTime = 0
        self._registerDevice("device_gz")
        self._setDevice("device_gz", { "count" : 0, "time": 0 })

        self.count = 0
        self.time.sleep(1)
        self.basicTime = 0

    def runLoop(self, timestep):
        self.time.sleep(0.0001)
        if self.count == 0:
            self.basicTime = self.tGz._sim_time
        self.simTime = self.tGz._sim_time - self.basicTime
        self._setDevice("device_gz", { "count":self.count, "time": self.simTime})
        self.count = self.count + 1

    def shutdown(self):
        self.t_os.system('killall gzserver')
        print("Engine 1 is shutting down")
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
        self.t_os.system("gzserver %s &" % self.modelFileName)

        self.simTime = 0
        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })

        self.count = 0

        import tClient
        self.tGz = tClient.GZ_Client()
        self.tGz.setListener('/gazebo/default/world_stats', 'none')
        #self.time.sleep(0.1)
        #self.tGz.toListen()
        #self.basicTime = self.tGz._sim_time
        
        #print(self.basicTime)

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        #if count == 0:
        #    self.basicTime = self.tGz._sim_time
        #self.tGz.toListen()
        self.time.sleep(0.0001)
        self.simTime = self.tGz._sim_time
        self._setDevice("device1", { "cmd" : self.count , "time": self.simTime})
        self.count  = self.count + 1
        print("Servo received data is " + str(self.simTime))

    def shutdown(self):
        print("Engine 1 is shutting down")
        self.t_os.system("killall -9 gzserver")
        self.tGz.shutdown()
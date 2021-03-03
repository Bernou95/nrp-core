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
        #self.t_os.system("gzserver %s &" % self.modelFileName)
        self.t_os.system("gzserver %s &" % self.modelFileName)
        self.simTime = 0
        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })


        self._registerDevice("rec_device2")
        self._setDevice("rec_device2", { "time": 0 })
        self.time.sleep(1)

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        #print(self.t_os.system("echo %s" % self.modelFileName))
        #print(self.modelFileName)
        #res = self.t_os.system("gz topic --echo %s &" % self.topicName)
        #print(res)
        self._setDevice("device1", { "cmd" : 1, "time": 1 })
        dev2 = self._getDevice("rec_device2")
        print("Servo received data is " + str(dev2))
        #print("Servo data is " + str(self._getDevice("device1")))

    def shutdown():
        self.t_os = self.importlib.import_module('killall gzserver')
        print("Engine 1 is shutting down")
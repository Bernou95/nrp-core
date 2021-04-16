"""Python Engine 1. Will get current engine time and make it accessible as a device"""
from NRPPythonEngineModule import EngineScript,RegisterEngine
from NRPPythonEngineModule import TMujocoLib

@RegisterEngine()
class Script(EngineScript):
    import math
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
      
        self.tMujoco = TMujocoLib("tosser.xml", True)
        
        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })
        self.count = 0

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        print("Sensor data: ", self.tMujoco.sim.data.sensordata)
        t_Time = self._time.count() / self.math.pow(10, 10)
        if self.count < 150:
            self.tMujoco.run_step(0.0)
        else:
            self.tMujoco.run_step(-1.0)
        self.count = self.count + 1
        self._setDevice("device1", { "cmd" : 1, "time": self.tMujoco.sim.data.time})
        half_mujoco_time = self.tMujoco.sim.data.time / 2 
        #print(self.tt.count())
        print("Time of CLE -->", t_Time)
        print("Half Time of Mujoco -->", half_mujoco_time)
        print("Servo data is " + str(self._getDevice("device1")))
        print("-------------------------------------------")

    def shutdown(self):
        print("Engine 1 is shutting down")



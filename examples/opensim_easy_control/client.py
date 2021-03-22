"""Python Engine 2. Will get time from Engine 1 and print it"""

from NRPPythonEngineModule import EngineScript,RegisterEngine


@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with empty time"""
        print("Client Engine is initializing. Registering device...")
        self._registerDevice("joint_data")
        self._setDevice("joint_data", { "shoulder" : 0, "elbow": 0})

        self.action = [0.0, 0.0, 0.0, 0.5, 0.0, 0.0] # arm 26
        self._registerDevice("action")
        self._setDevice("action", { "act_list": self.action})

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        #'''
        joints = self._getDevice("joint_data")

        if joints.get("elbow") > 2.0:
            self.action = [0.0, 0.2, 0.2, 0.2, 0.0, 0.0]
        elif joints.get("elbow") > 1.0:
            self.action = [0.0, 0.0, 0.4, 0.4, 0.4, 0.0]
        else:
            self.action = [0.0, 0.0, 0.0, 0.6, 0.6, 0.6]
        #print(joints, self.action)
        self._setDevice("action", { "act_list": self.action})

    def shutdown(self):
        print("Engine 2 is shutting down")

    


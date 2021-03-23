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

        self.e_i = 0
        self.e_old = 0

        self.count = 0
        self.u = 0
        self.kp = 0.33
        self.ki = 0.000005
        self.kd = 0.99

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        #'''
        joints = self._getDevice("joint_data")
        elbow_joint = joints.get("elbow")

        # Easy state machine
        '''
        if elbow_joint > 2.0:
            self.action = [0.0, 0.2, 0.2, 0.2, 0.0, 0.0]
        elif elbow_joint > 1.0:
            self.action = [0.0, 0.0, 0.4, 0.4, 0.4, 0.0]
        else:
            self.action = [0.0, 0.0, 0.0, 0.6, 0.6, 0.6]
        '''
        # Easy PID controller
        #'''
        if self.count % 5 == 0:
            print('----------------------------------------------------> ')
            err = 2.0 - elbow_joint
            self.u = self.pid_ctrl(err)
        self.count = self.count + 1
        self.action = [0.5-self.u, 0.0, 0.0, 0.0, 0.5+self.u, 0.0]
        #'''
        self._setDevice("action", { "act_list": self.action})

    def shutdown(self):
        print("Engine 2 is shutting down")


    def pid_ctrl(self, error):
        self.e_i = self.e_i + error
        de = error - self.e_old
        u = self.kp*error + self.ki*self.e_i  + self.kd*de
        self.e_old = error

        return u

    


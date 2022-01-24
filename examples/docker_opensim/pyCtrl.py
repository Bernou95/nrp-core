"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

from nrp_core.engines.python_json import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize datapack1 with time"""
        print("Engine 1 is initializing. Registering datapack...")
        self._registerDataPack("info")
        self._setDataPack("info", {"joints" : {}, "forces": {}})

        self._registerDataPack("act_list")
        self.action = [0.0, 0.0, 0.0, 0.5, 0.0, 0.0]
        self._setDataPack("act_list", { "action": self.action, "reset": 0})

        self.e_i = 0
        self.e_old = 0

        self.count = 0
        self.u = 0
        self.kp = 0.39
        self.ki = 0.001
        self.kd = 0.99

        self.reset_count = 0

    def runLoop(self, timestep):
        """Receive datapack1 at every timestep"""
        #'''
        elbow_joint = 0
        tJoint = self._getDataPack("info").get("joints")
        if tJoint == None:
            elbow_joint = 0
        else:
            elbow_joint = tJoint["r_elbow"]
        print("elbow joint data is --> ", elbow_joint)
        if self.count % 4 == 0:
            err = 2.0 - elbow_joint
            self.u = self.pid_ctrl(err)
        self.count = self.count + 1
        self.action = [0.5-self.u, 0.0, 0.0, 0.5+self.u, 0.0, 0.0]
        #'''
        reset_flag = 0
        self.reset_count = self.reset_count + 1
        # The reset need two step: one step to start the reset
        # and one step to wait the end of reset
        if self.reset_count % 200 < 2: 
            if self.reset_count % 200 == 0:
                reset_flag = 1
            self.action = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
            self.para_init()
        self._setDataPack("act_list", { "action": self.action, "reset": reset_flag})


    def shutdown(self):
        print("Engine 1 is shutting down")

    def reset(self):
        print("Engine 1 is resetting")


    def para_init(self):
        self.e_i = 0
        self.e_old = 0

        self.count = 0
        self.u = 0
        self.kp = 0.39
        self.ki = 0.001
        self.kd = 0.99

    def pid_ctrl(self, error):
        self.e_i = self.e_i + error
        de = error - self.e_old
        u = self.kp*error + self.ki*self.e_i  + self.kd*de
        self.e_old = error

        return u

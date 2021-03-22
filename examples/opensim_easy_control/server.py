"""Python Engine 1. Will get current engine time and make it accessible as a device"""
from NRPPythonEngineModule import EngineScript,RegisterEngine
from NRPPythonEngineModule import TOpenSim

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with time"""
        print("Server Engine is initializing. Registering device...")
        self.tOS = TOpenSim("arm26/arm26_ground_offset.osim", True)
        
        self._registerDevice("joints")
        self._setDevice("joints", { "shoulder" : 0, "elbow": 0 })
        self._registerDevice("infos")
        self._setDevice("infos", { "time" : 0})

        self.action = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        self._registerDevice("control_cmd")
        self._setDevice("control_cmd", { "act_list" : self.action})

    def runLoop(self, timestep):

        jointSet = self.tOS.model.getJointSet();
        shoulder = jointSet.get(1).getCoordinate()
        elbow = jointSet.get(2).getCoordinate()

        s_val = shoulder.getValue(self.tOS.state)
        e_val = elbow.getValue(self.tOS.state)
        self._setDevice("joints", {"shoulder": s_val, "elbow": e_val})
        self._setDevice("infos", {"time": self.tOS.state.getTime()})

        self.action = self._getDevice("control_cmd").get("act_list")
        #self.action = [0.0, 0.0, 0.0, 0.5, 0.5, 0.5]
        self.tOS.run_step(self.action)
        
        # To show components in the model changed by action
        # 1: To show components in a list
        #ctrl_list = self.tOS.model.getControlsTable()
        '''
        # 2: To show components one by one
        ctrlSet = self.tOS.model.getForceSet()
        for j in range(ctrlSet.getSize()):
            print('|', ctrlSet.get(j).getName(), end=' ')
        print('|--> Action size: ', ctrlSet.getSize())
        '''
    def shutdown(self):
        print("Engine 1 is shutting down")



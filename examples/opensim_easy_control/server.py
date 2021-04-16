"""Python Engine 1. Will get current engine time and make it accessible as a device"""
from NRPPythonEngineModule import EngineScript,RegisterEngine
from NRPPythonEngineModule import TOpenSim

# The API of Opensim is shown in the following link:
# https://simtk.org/api_docs/opensim/api_docs/classOpenSim_1_1Model.html

@RegisterEngine()
class Script(EngineScript):
    import math
    def initialize(self):
        """Initialize device1 with time"""
        print("Server Engine is initializing. Registering device...")
        self.tOS = TOpenSim("arm26/arm26_ground_offset.osim", True)
        print("Registering device --> for sensors")
        self._registerDevice("joints")
        self._setDevice("joints", { "shoulder" : 0, "elbow": 0 })
        self._registerDevice("infos")
        self._setDevice("infos", { "time" : 0})

        # To set the force of muscles, in arm_26, they are:
        # ['TRIlong', 'TRIlat', 'TRImed', 'BIClong', 'BICshort', 'BRA']
        # The default color of muscle in the visualizer is blue.
        # Once the force of a muscle is not the default value, 
        # the color of the muscle will be changed. 
        # Using this phenomenon, the controlled muscles can be found in the visualizer
        # For example, if action=[0.5, 0.0, 0.0, 0.0, 0.0, 0.0], 
        # the color of TRIlong will not be blue in shown screen
        self.action = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        print("Registering device --> for actuators")
        self._registerDevice("control_cmd")
        self._setDevice("control_cmd", { "act_list" : self.action})

    def runLoop(self, timestep):
        # All Joints and Muscles can be found in the "*.osim"
        # Obtain the joint data from model "arm_26"
        # In arm_26, the joint set is [offset, r_shoulder, r_elbow]
        s_val = self.tOS.getDeviceValue("r_shoulder", deviceType="Joint")
        e_val = self.tOS.getDeviceValue("r_elbow", deviceType="Joint") 
        # Send data to TF
        self._setDevice("joints", {"shoulder": s_val, "elbow": e_val})
        self._setDevice("infos", {"time": self.tOS.state.getTime()})
        
        #print("Time: ", self.tOS.state.getTime())

        # Receive control data from TF
        self.action = self._getDevice("control_cmd").get("act_list")
        # Set muscles' force to change joints
        self.tOS.run_step(self.action, timestep.count())
        
        # To show components in the model changed by action
        # 1: To show components in a list
        #ctrl_list = self.tOS.model.getControlsTable()
        # 2: To show components one by one
        #print(self.tOS.getNameSet("Force"))

    def shutdown(self):
        print("Engine 1 is shutting down")



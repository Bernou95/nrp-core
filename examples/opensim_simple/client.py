"""Python Engine 2. Will get time from Engine 1 and print it"""

from NRPOpensimEngineModule import EngineScript,RegisterEngine


@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        import opensim as osim
        """Initialize device1 with empty time"""
        print("Client Engine is initializing. Registering device...")
        self.modelFileName = self._world
        #'''
        #modelFileName = 'calibrated_Rajagopal_2015.osim'; 
        orientationsFileName = 'MT_012005D6_009-001_orientations.sto';
        self.imuIK = osim.IMUInverseKinematicsTool();
        self.imuIK.set_model_file(self.modelFileName);
        #self.imuIK.set_orientations_file(orientationsFileName);  
        #'''
        self._registerDevice("rec_device1")
        self._setDevice("rec_device1", { "cmd" : 0, "time": 0 })

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        #'''
        # Set time range in seconds
        self.imuIK.set_time_range(0, 0); 
        self.imuIK.set_time_range(1, 3);   
        # Run IK
        
        self.imuIK.run(True);
        #'''
        dev1 = self._getDevice("rec_device1")
        print("Engine 2 received data is " + self.modelFileName)
        print("Engine 2 received data is " + str(dev1))

    def shutdown():
        print("Engine 2 is shutting down")

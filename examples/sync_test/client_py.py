"""Python Engine 2. Will get time from Engine 1 and print it"""

from NRPPythonEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with empty time"""
        self.storeFile = open("EningeTimes.txt", "w")

        self.engine_num = 4
        self.oldList = [0.0]*self.engine_num

        eName_1 = "|E1= Gazebo"
        eName_2 = "|E2= Nest  "
        eName_3 = "|E3= Python"
        eName_4 = "|E4= Python"
        self.enginesName = eName_1 + eName_2 + eName_3 + eName_4 + "|"
        self.storeFile.write(self.enginesName+'\n')
        print(self.enginesName)

        self._registerDevice("rec_sync_test")
        self._setDevice("rec_sync_test", {"times":[0.0]*self.engine_num})

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        dev1 = self._getDevice("rec_sync_test")
        newList = dev1.get("times")
        #print(newList)
        tStr = ""
        for i in range(self.engine_num):
            tStr = tStr + "|E"+ str(i+1) + ": "
            tStr = tStr + ('%.3f'%newList[i])
            if newList[i] != self.oldList[i]:
                tStr = tStr + "+"
            else:
                tStr = tStr + " "
            self.oldList[i] = newList[i]
        tStr = tStr + "|"
        self.storeFile.write(tStr+'\n')
        print(tStr)

        #print("Engine 2 received data is " + str(dev1))

    def shutdown(self):
        self.storeFile.write(self.enginesName+'\n')
        self.storeFile.close()
        print(self.enginesName)

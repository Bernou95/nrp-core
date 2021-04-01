
# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020 Michael Zechmair
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This project has received funding from the European Union’s Horizon 2020
# Framework Programme for Research and Innovation under the Specific Grant
# Agreement No. 945539 (Human Brain Project SGA3).

"""Simple python script file"""

from NRPPythonEngineModule import EngineScript,RegisterEngine


@RegisterEngine()
class Script(EngineScript):
    from TOsimLib import TOpenSim
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.tOS = self.TOpenSim("test_osim/arm26_ground_offset.osim", False)
        self._registerDevice("device1")
        self._setDevice("device1", {"time":0, "time_1":0})

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        #print("1 ---> ")
        self.tOS.run_step([0.0, 0.0, 0.0, 0.5, 0.0, 0.0], timestep.count())
        #print("2 ---> ")
        self._setDevice("device1", {"time":self.tOS.state.getTime(), "time_1":0})
        #self._setDevice("device1", { "cmd" : 1, "time": self.tOS.state.getTime()})
        print("Step End ---> ")
        print("Engine 1 at time " + str(self.tOS.state.getTime()))

    def shutdown(self):
        print("Engine 1 is shutting down")
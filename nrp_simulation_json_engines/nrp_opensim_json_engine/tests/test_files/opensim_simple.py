
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

from NRPOpensimEngineModule import EngineScript,RegisterEngine
import TOsimLib

'''
@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with time"""
        print("Servo Engine is initializing. Registering device...")
        self.modelFileName = self._world
        #self.tOS = TOpenSim(self.modelFileName)
        self._registerDevice("device1")
        self._setDevice("device1", { "cmd" : 0, "time": 0 })

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        print("Engine 1 at time " + str(self._time.count()))

    def shutdown():
        print("Engine 1 is shutting down")
'''
@RegisterEngine()
class Script(EngineScript):
    """Simple script function"""
    def runLoop(self, timestep):
        print("Engine 1 at time " + str(self._time.count()))
#'''
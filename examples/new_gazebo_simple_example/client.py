"""Python Engine 2. Will get time from Engine 1 and print it"""

from NRPNewGazeboEngineModule import EngineScript,RegisterEngine


@RegisterEngine()
class Script(EngineScript):
    import importlib
    import math
    import asyncio
    import pygazebo
    import time
    def initialize(self):
        """Initialize device1 with empty time"""
        print("Client Engine is initializing. Registering device...")
        self._registerDevice("device2")
        self._setDevice("device2", {"time": -1 })

        self._registerDevice("rec_device1")
        self._setDevice("rec_device1", { "cmd" : 0, "time": 0 })
        self.time.sleep(1)

        self.timeInfo = self.importlib.import_module('pygazebo.msg.world_stats_pb2')
        self.simTime = 0
        self.loop = self.asyncio.new_event_loop()
        self.asyncio.set_event_loop(self.loop)

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        #'''
        self.loop.run_until_complete(self.sub_loop())
        dev1 = self._getDevice("rec_device1")
        self._setDevice("device2", { "cmd" : 1, "time": self.simTime })
        print("Client received data is " + str(dev1))

    def shutdown():
        print("Engine 2 is shutting down")
    #'''
    async def sub_loop(self):
        manager = await self.pygazebo.connect(('localhost', 12345))
        def callback(data):
            message = self.timeInfo.WorldStatistics.FromString(data)        
            sim_time = message.sim_time
            self.simTime = sim_time.sec + sim_time.nsec/self.math.pow(10, 9)
            #print(sim_time)
        subscriber = manager.subscribe('/gazebo/default/world_stats',
            'gazebo.msgs.WorldStatistics', callback)

        await subscriber.wait_for_connection()
        await self.asyncio.sleep(0.1)
    #'''

    


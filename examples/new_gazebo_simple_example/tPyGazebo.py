#from trollius import From
import asyncio
import pygazebo

import pygazebo.msg.world_stats_pb2

import time
'''
def callback(data):
	message = pygazebo.msg.world_stats_pb2.WorldStatistics.FromString(data)        
	print(message)

manager_future = pygazebo.connect(('localhost', 12345))
asyncio.get_event_loop().run_until_complete(manager_future)
manager = manager_future.result()

subscriber = manager.subscribe('/gazebo/default/world_stats', 
	'gazebo.msgs.WorldStatistics', callback)

async def sub_loop():
	await subscriber.wait_for_connection()
	while(sub_loop.is_waiting):
		await asyncio.sleep(1.0)
	print('Done waiting!')

manager.start()

'''
#'''
async def sub_loop():
	manager = await pygazebo.connect(('localhost', 12345))
	sub_loop.is_waiting = True
	def callback(data):
		#print(data)
		if not sub_loop.is_waiting:
			return
		message = pygazebo.msg.world_stats_pb2.WorldStatistics.FromString(data)        
		sim_time = message.sim_time
		import math
		sim_time = sim_time.sec + sim_time.nsec/math.pow(10, 9)
		print(sim_time)

	subscriber = manager.subscribe('/gazebo/default/world_stats',
		'gazebo.msgs.WorldStatistics', callback)

	#await subscriber.wait_for_connection()
	await asyncio.sleep(1.0)
	#while(True):
	#	await asyncio.sleep(1.0)
	#print('Done waiting!')

#asyncio.run(sub_loop())
loop = asyncio.get_event_loop()
#time.sleep(1.0)
loop.run_until_complete(sub_loop())
#'''

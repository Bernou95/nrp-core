from tClient import *
import _thread
import time

print(hello())

tGz = GZ_Client()
tGz.setListener('/gazebo/default/world_stats', 'none')
print(tGz.value)
for i in range(10):
	tGz.toListen()
	print(tGz._sim_time)
	print(tGz._msg)
	print("-----------------------------")
	time.sleep(1)
	#print(tGz.value)
tGz.shutdown()

from tServer import *
import _thread
import time
from threading import Thread
import sys
import os
print(hello())

#tGz = GZ_Server('camera_check.sdf')
tGz = GZ_Server()
tGz.setWorld('camera_check.sdf')
#tGz.loadWorld('camera_check.sdf')


#th = _thread.start_new_thread(tGz.loadWorld,())
tGz.loadWorld('camera_check.sdf')
for i in range(3):
	print(i)
	time.sleep(1)
tGz.shutdown()


from .ClientLib import ClientInterface
from .EnvSet import *
import time
import json
import sys

import docker
from docker import DockerClient

class NetConnector(object):
	"""
	This class will receive the information simulator need from NRP engine script,
	and then start and run different simulators with python API
	"""
	def __init__(self, configureStr):
		super(NetConnector, self).__init__()
		# Server
		self.tStr = configureStr
		self.configureVal = json.loads(self.tStr)

		ipHost = self.configureVal["IPHost"]
		ipPort = self.configureVal["Port"]
		self.client = DockerClient(base_url='tcp://'+ipHost+':'+str(ipPort))
		result = self.client.images.list()
		simulator = self.configureVal["Simulator"]
		#'''
		print("Waiting for container start !!!")
		container = self.client.api.create_container(
			image=simulator+':v0',
			name="exp_sim",
			tty=True)
		
		self.client.api.start(container)
		print("Container is started !!!")
		self.container_id = container['Id']
		
		ip_add = self.client.containers.get("exp_sim").attrs['NetworkSettings']['IPAddress']
		result = self.client.containers.get("exp_sim").exec_run(
			"python3 main.py", tty=True, stream=True, 
			environment=OPENSIM_ENV,
			workdir="/root/simulator/LocalModel")
		time.sleep(2)
		host = ip_add
		port = PORT_NUM
		#'''
		# Client
		
		time_step = self.configureVal["EngineTimestep"]
		world_file = self.configureVal["WorldFileName"]
		start_visualizer = bool(self.configureVal["Visualizer"])
		config = {
			"EngineTimestep": time_step,
			"WorldFileName":world_file,
			"Visualizer":start_visualizer
		}

		self.net_interface = ClientInterface(host, port)
		self.net_interface.sendJSONMsg(config)
		msg_data = self.net_interface.receiveMsg()
		print(msg_data)

	# TODO: currently this function is not connected to reset requests from the Engine client. The PythonJSONEngine
	#  server is just shutting down and initializing again the Engine. Calling this function if it is available would be
	#  more efficient
	def run_step(self,actList):
		self.net_interface.sendStrMsg(actList)
		msg_data = self.net_interface.receiveMsg()
		return msg_data

	def reset(self):
		self.net_interface.sendStrMsg("RESET")
		msg_data = self.net_interface.receiveMsg()


	def shutdown(self):
		#self.net_interface.sendStrMsg("END")
		#msg_data = self.net_interface.receiveMsg()
		self.client.api.kill(self.container_id)
		self.client.api.remove_container(self.container_id)



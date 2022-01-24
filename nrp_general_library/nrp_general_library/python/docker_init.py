import time
import json
import sys
import requests

import docker
from docker import DockerClient

OPENSIM_ENV = ["LD_LIBRARY_PATH=/Opensim/OpenSim/lib:$LD_LIBRARY_PATH",
			"PYTHONPATH=/Opensim/OpenSim/lib/python3.8/site-packages:$PYTHONPATH"]

class DockerConnector(object):
	"""
	This class will receive the information simulator need from NRP engine script,
	and then start and run different simulators with python API
	"""
	def __init__(self, configureStr):
		super(DockerConnector, self).__init__()
		# Server
		self.tStr = configureStr
		self.configureVal = json.loads(self.tStr)

		ipHost = self.configureVal["IPHost"]
		self.client = DockerClient(base_url='tcp://'+ipHost)
		result = self.client.images.list()
		simulator = self.configureVal["Simulator"]
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

		response = requests.post('http://'+ip_add+':5000//init', json = self.configureVal)
		time.sleep(2)
		print(response.json())

	def getIpAdd(self):
		ipAddr = self.client.containers.get("exp_sim").attrs['NetworkSettings']['IPAddress']
		port = ":"+str(5000)
		return ipAddr + port
	def shutdown(self):
		print("Shutdown !!!")
		self.client.api.kill(self.container_id)
		self.client.api.remove_container(self.container_id)



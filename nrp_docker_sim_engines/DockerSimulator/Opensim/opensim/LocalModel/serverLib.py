from OpensimLib import OpensimInterface

import socket
import json
import sys
import time

class TheServer(object):
	"""docstring for theServer"""
	BUFFSIZE = 1024
	def __init__(self, HOST, PORT):
		super(TheServer, self).__init__()
		address = (HOST, PORT)
		self.trans = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.trans.bind(address)
		self.trans.listen(5)
		self.connector, addr = self.trans.accept()
		print('[+] Connected with', addr)

	def receiveMsg(self):
		try:
			data = self.connector.recv(self.BUFFSIZE)
			time.sleep(0.0001)
		except ConnectionResetError:
			exit()
		data = data.decode()
		return data

	def sendStrMsg(self, msg):
		self.connector.sendall(msg.encode())
	def sendJSONMsg(self, msg):
		data = json.dumps(msg)
		self.connector.sendall(data.encode())

	def shutdown(self):
		self.connector.close()
		self.trans.close()

class ForSimulator(object):
	"""docstring for ForSimulator"""
	def __init__(self, HOST, PORT):
		super(ForSimulator, self).__init__()
		self.osimServer = TheServer(HOST, PORT)
		print("Waitting configuration information !!!")
		msg_data = self.osimServer.receiveMsg()
		while not msg_data:
			print("Network error, please resend !!!")
			msg_data = self.osimServer.receiveMsg()
		print(msg_data)
		_config = json.loads(msg_data)
		self.sim_interface = OpensimInterface(
			_config.get("WorldFileName"),
			_config.get("Visualizer"),
			_config.get("EngineTimestep"))
		self.osimServer.sendStrMsg("Finish Connection !!!")
		print("Finish Initialization !!!")

	def msgOperation(self, msg_data):
		if msg_data == "END":
			self.shutdown()
			return 0
		elif msg_data == "RESET":
			self.sim_interface.reset()
			self.osimServer.sendStrMsg("Reset !!!")
			print("Reset !!!")
			return 1
		else:
			dictData = json.loads(msg_data)
			resetFlag = int(dictData.get("reset"))
			if resetFlag == 1:
				self.sim_interface.reset()

			self.sim_interface.run_one_step(dictData.get("action"))
			sendData = {"joints" : {}, "forces": {}}
			sendData["joints"] = self.sim_interface.get_model_properties_val("Joint")
			sendData["forces"] = self.sim_interface.get_model_properties_val("Force")
			tJson = json.dumps(sendData, ensure_ascii=False)
			self.osimServer.sendStrMsg(tJson)
			return 2
	
	def runLoop(self):
		while True:
			msg_data = self.osimServer.receiveMsg()
			flag = self.msgOperation(msg_data)
			if flag == 0:
				break

	def shutdown(self):
		self.osimServer.sendStrMsg("END !!!")
		self.osimServer.shutdown()
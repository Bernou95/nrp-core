import socket
import json
import sys

class ClientInterface(object):
	"""docstring for TheClient"""
	BUFFSIZE = 1024
	def __init__(self, HOST, PORT):
		super(ClientInterface, self).__init__()
		address = (HOST, PORT)
		self.trans = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			self.trans.connect(address)
		except Exception:
			print('[!] Server not found ot not open')
			sys.exit()

	def receiveMsg(self):
		data = self.trans.recv(self.BUFFSIZE)
		data = data.decode()
		return data

	def sendJSONMsg(self, msg):
		data = json.dumps(msg)
		self.trans.sendall(data.encode())
	def sendStrMsg(self, msg):
		self.trans.sendall(msg.encode())

	def shutdown(self):
		self.trans.close()


from serverLib import ForSimulator

if __name__ == '__main__':
	print("Waitting to connect !!!")
	HOST, PORT = '0.0.0.0', 5005
	tServer = ForSimulator(HOST, PORT)
	print("Simulating !!!")
	tServer.runLoop()
	print("Shutdown !!!")



	
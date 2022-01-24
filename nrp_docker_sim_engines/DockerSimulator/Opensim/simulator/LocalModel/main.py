from flask import Flask,request, jsonify
from flask_socketio import SocketIO,emit

from OpensimLib import OpensimInterface
import json

app = Flask(__name__)

socketio = SocketIO()
socketio.init_app(app)
global simVal

@app.route('/')
def index():
	app.logger.info('Hello world!')

@app.route('/init', methods=['POST'])
def initSim():
	global simVal

	_config = request.json
	app.logger.info(_config)
	sim_interface = OpensimInterface(
		_config.get("WorldFileName"),
		_config.get("Visualizer"),
		_config.get("EngineTimestep"))
	simVal = sim_interface
	return jsonify("Finish Initialization !!!")

@app.route("/shutdown", methods=['GET'])
def shutdown():
	app.logger.info('Shutting down...')
	socketio.stop()
	return "Shutting down..."

@app.route('/transfer', methods=['POST'])
def postMsg():
	global simVal

	dictData = request.json
	app.logger.info(dictData)
	resetFlag = int(dictData.get("reset"))
	if resetFlag == 1:
		simVal.reset()

	app.logger.info(dictData.get("action"))
	simVal.run_one_step(dictData.get("action"))
	sendData = {"joints" : {}, "forces": {}}
	sendData["joints"] = simVal.get_model_properties_val("Joint")
	sendData["forces"] = simVal.get_model_properties_val("Force")
	
	return jsonify(sendData)

if __name__ == '__main__':
	socketio.run(app,debug=True,host='0.0.0.0',port=5000)
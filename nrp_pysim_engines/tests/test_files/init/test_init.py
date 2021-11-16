from nrp_core.engines.opensim import OpenSimEngineScript
from nrp_core.engines.python_json import RegisterEngine

@RegisterEngine()
class Script(OpenSimEngineScript):
	def runLoop(self, timestep):
		print("Engine 1 at time " + str(self._time.count()))
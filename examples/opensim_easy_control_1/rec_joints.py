from NRPPythonModule import *

@FromEngineDevice(keyword='joints', id=DeviceIdentifier('joints', 'server'))
@FromEngineDevice(keyword='infos', id=DeviceIdentifier('infos', 'server'))
@TransceiverFunction("client")
def transceiver_function(infos, joints):
    rec_joints = PythonDevice("joint_data", "client")
    rec_joints.data = joints.data

    return [rec_joints]

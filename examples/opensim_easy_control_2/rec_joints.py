from NRPPythonModule import *

@FromEngineDevice(keyword='joints', id=DeviceIdentifier('joints', 'server'))
@FromEngineDevice(keyword='infos', id=DeviceIdentifier('infos', 'server'))
@TransceiverFunction("server")
def transceiver_function(infos, joints):
    action = PythonDevice("control_cmd", "server")
    rec_joints = joints.data
    elbow_joint = rec_joints.get("elbow")
    print(elbow_joint)

    action.data.set("act_list", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
    return [action]

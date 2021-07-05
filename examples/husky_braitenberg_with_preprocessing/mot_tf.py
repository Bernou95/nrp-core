from NRPPythonModule import *
<<<<<<< HEAD
from NRPProtoPythonModule import JointDevice
=======
from NRPGazeboDevicesPython import PhysicsJoint
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

@FromEngineDevice(keyword='actors', id=DeviceIdentifier('actors', 'nest'))
@TransceiverFunction("gazebo")
def transceiver_function(actors):
<<<<<<< HEAD
    back_left_j   = JointDevice("husky::back_left_joint", "gazebo")
    back_right_j  = JointDevice("husky::back_right_joint", "gazebo")
    front_left_j  = JointDevice("husky::front_left_joint", "gazebo")
    front_right_j = JointDevice("husky::front_right_joint", "gazebo")

    left_voltage = actors.data[0]['V_m']
    right_voltage = actors.data[1]['V_m']

    forward_vel = 20.0 * min(left_voltage, right_voltage)
    rot_vel = 70.0 * (right_voltage - left_voltage)

    back_left_j.data.velocity = (forward_vel - rot_vel) * 10
    back_right_j.data.velocity = (forward_vel + rot_vel) * 10
    front_left_j.data.velocity = (forward_vel - rot_vel) * 10
    front_right_j.data.velocity = (forward_vel + rot_vel) * 10

    print("------------------")
    print("Left voltage:  " + str(back_left_j.data.velocity))
    print("Right voltage: " + str(back_right_j.data.velocity))
    print("Forward velocity: " + str(front_left_j.data.velocity))
    print("Rotational vel:   " + str(front_right_j.data.velocity))
=======
    back_left_j   = PhysicsJoint("husky::back_left_joint", "gazebo")
    back_right_j  = PhysicsJoint("husky::back_right_joint", "gazebo")
    front_left_j  = PhysicsJoint("husky::front_left_joint", "gazebo")
    front_right_j = PhysicsJoint("husky::front_right_joint", "gazebo")

    left_voltage = actors.data[0]['V_m']
    right_voltage = actors.data[1]['V_m']
    
    forward_vel = 20.0 * min(left_voltage, right_voltage)
    rot_vel = 70.0 * (right_voltage - left_voltage)

    back_left_j.velocity = (forward_vel - rot_vel) * 10
    back_right_j.velocity = (forward_vel + rot_vel) * 10
    front_left_j.velocity = (forward_vel - rot_vel) * 10
    front_right_j.velocity = (forward_vel + rot_vel) * 10

    print("Left voltage:  " + str(left_voltage))
    print("Right voltage: " + str(right_voltage))

    print("Forward velocity: " + str(forward_vel))
    print("Rotational vel:   " + str(rot_vel))
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    # return []
    return [ back_left_j, back_right_j, front_left_j, front_right_j ]


from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboJointDataPack
from math import pi

# with open('output.csv', 'w') as f:
#     pass

# @EngineDataPack(keyword='spiker', id=DataPackIdentifier('spiker', 'nest'))
@EngineDataPack(keyword='actors', id=DataPackIdentifier('actors', 'nest'))
@EngineDataPack(keyword='left_eye', id=DataPackIdentifier('icub_pid::left_eye_pan', 'gazebo'))
@EngineDataPack(keyword='right_eye', id=DataPackIdentifier('icub_pid::right_eye_pan', 'gazebo'))
@TransceiverFunction("gazebo")
def robot_control(left_eye, right_eye, actors): #  , spiker):

    def deg2rad(deg):
        return (deg / 360.) * (2. * pi)

    # Process input
    eye_version = (left_eye.data.position + right_eye.data.position) / 2.0
    eye_vergence = left_eye.data.position - right_eye.data.position
    left_voltage = actors.data[0]['V_m']
    right_voltage = actors.data[1]['V_m']
    d = right_voltage - left_voltage

    # Compute output
    max_mov = 1.0
    eye_version_ret = eye_version + 2 * deg2rad(-((d + 0.03) / 0.09 * 2 * max_mov - max_mov))

    # TODO: can I just return input datapacks?
    left_eye_ret = GazeboJointDataPack("icub_pid::left_eye_pan", "gazebo")
    right_eye_ret = GazeboJointDataPack("icub_pid::right_eye_pan", "gazebo")

    left_eye_ret.data.position = eye_version_ret + (eye_vergence / 2.0)
    right_eye_ret.data.position = eye_version_ret - (eye_vergence / 2.0)


    # global t
    # print(t)
    # with open('output.csv', 'a') as f:
    #     f.write('{},{},{},{}\n'.format(t,d,left_voltage, right_voltage))
    # if t >= 9.99:
    #     with open('spikes.csv', 'w') as f:
    #         for i in range(len(spiker.data[0]['events']['senders'])):
    #             f.write('{},{}\n'.format(spiker.data[0]['events']['senders'][i], spiker.data[0]['events']['times'][i]))
    # with open('output.csv', 'a') as f:
    #     f.write('{},{},{},{}\n'.format(t,d,eye_version_ret, right_eye_ret.data.position))
    #
    # return []

    return [left_eye_ret, right_eye_ret]

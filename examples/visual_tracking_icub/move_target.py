from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboLinkDataPack
from math import sin, pi

frequency = 0.3
amplitude = 0.3
center = {'x': 0, 'y': 2.42, 'z': 1.2}
t = 0

# with open('target.csv', 'w') as f:
#     pass

@TransceiverFunction("gazebo")
def robot_control():
    global t

    target = GazeboLinkDataPack("target::Target", "gazebo")
    target.data.position.append(center['x'] + sin(t * frequency * 2 * pi) * (amplitude / 2))
    target.data.position.append(center['y'])
    target.data.position.append(center['z'])

    # with open('target.csv', 'a') as f:
    #     f.write('{},{}\n'.format(t, target.data.position[0]))

    t += 0.01

    return [target]

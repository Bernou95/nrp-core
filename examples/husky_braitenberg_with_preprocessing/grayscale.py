from NRPPythonModule import *
<<<<<<< HEAD
import numpy as np
=======
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

@FromEngineDevice(keyword='camera', id=DeviceIdentifier('husky_camera::camera', 'gazebo'))
@PreprocessingFunction("gazebo")
def transceiver_function(camera):

    # Return an empty device, if there's no data in camera device
    if camera.isEmpty():
        return [ DeviceInterface("processed", "gazebo") ]

    # Convert image to grayscale
    rgb_weights = [0.2989, 0.5870, 0.1140]
<<<<<<< HEAD
    d = np.frombuffer(camera.data.imageData, np.uint8)
    image_data = d.reshape((camera.data.imageHeight,camera.data.imageWidth,3))

    my_dict = {
        "image_height": camera.data.imageHeight,
        "image_width" : camera.data.imageWidth,
=======
    image_data = camera.image_data.reshape((camera.image_height, camera.image_width, 3))

    my_dict = {
        "image_height": camera.image_height,
        "image_width" : camera.image_width,
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
        "grayscale"   : np.dot(image_data[...,:3], rgb_weights),
    }

    # Save image in grayscale in a device and return it
    device = PythonDevice("processed", "gazebo")
    device.data = my_dict

    return [ device ]

import opensim as osim
import math
# Set variables to use
modelFileName = 'arm26.osim';                # The path to an input model
orientationsFileName = 'MT_012005D6_009-001_orientations.sto';   # The path to orientation data for calibration 
sensor_to_opensim_rotation = osim.Vec3(-math.pi/2, 0, 0); # The rotation of IMU data to the OpenSim world frame 
visualizeTracking = True;  # Boolean to Visualize the tracking simulation
startTime = 7.25;          # Start time (in seconds) of the tracking simulation. 
endTime = 15;              # End time (in seconds) of the tracking simulation.
resultsDirectory = 'IKResults';

# Instantiate an InverseKinematicsTool
imuIK = osim.IMUInverseKinematicsTool();
 
# Set tool properties
imuIK.set_model_file(modelFileName);
#imuIK.set_orientations_file(orientationsFileName);
#imuIK.set_sensor_to_opensim_rotations(sensor_to_opensim_rotation)
#imuIK.set_results_directory(resultsDirectory)

# Set time range in seconds
imuIK.set_time_range(0, startTime); 
imuIK.set_time_range(1, endTime);   

# Run IK
imuIK.run(visualizeTracking);

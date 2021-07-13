Step 1: Entry the folder "Not - NVIDIA" or "With -NVIDIA" and run the script
"env_config.sh" 
to install the dependency for visualization

Step 2: Entry the folder "DockerFile" and execute:
"docker build -t opensim:v1 ."
to install the docker image from dockerfile.
In addition, opensim:v1 is the name and tag of the docker image

Step 3: Wait the docker image installation completed, about 2 hours or more


Step 4: Check whether the docker image is installed, using command
"docker images", 
and if the image is installed, you can find a docker image name "opensim"

Step 5: Entry the folder "Not - NVIDIA" or "With -NVIDIA" and run the script
"runDocker.sh" 
to entry the docker container.
(Note, the "IMAGENAME" in the scripts need to be changed to your own docker image name, just like "opensim:v1" as mentioned before)

 Step 6: Run the test file in the docker container, using command
"./runTestOpensim.sh "

Step 7: Exit the docker container:
"exit"

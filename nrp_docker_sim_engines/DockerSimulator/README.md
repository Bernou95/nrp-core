Step 1: build 2 virtual machine and install docker respectively

	For example: To build the image for nrp-core, we can use command "docker build -t nrp:v0 ." inside the floder "NRP". "nrp:v0" is the name and tag of the docker image.

Step 2: Install the simulator container in Follower VM, install NRP Container in Manager VM

Step 3: Config the  Follower  VM

	Change “/usr/lib/systemd/system/docker.service”, we can find one sentence “ExecStart = …” in this file. Add “-H tcp://0.0.0.0:2375” at the end of this sentence. To may the port "2357" as a public port.
	
	Open the port to firewall
	
	sudo apt-get install firewalld
	
	sudo firewall-cmd --add-port=2375/tcp --permanent
	
	sudo firewall-cmd –reload execu

To execute the example:

Open all Manager VM and follower VM. For each machine, it needs use "sudo firewall-cmd --add-port=2375/tcp --permanent" to open the same pubilc port. And for follower VM, it need do the change in docker.service.

In the Manager VM, use the docker images to run docker container. 

sudo docker run –name nrpnode -it nrp:v0

Inside the docker container, go to nrp-core/example/docker_opensim, and run

NRPCoreSim -c simulation_config.json -p "NRPDockerSimEngine.so"

And the simulation will be run with NRP

Pay attention, the IP address of different VM may be different. If it connect fail when nrp working, it may be need to change the ip address. And it can be changed in "simulation_config.json" as "IPHost" and "Port". And the IP address need to be the same with then  docker container running in the Follower VM.

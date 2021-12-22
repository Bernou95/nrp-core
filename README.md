Step 1: build 2 virtual machine and install docker respectively


Step 2: Install the simulator container in Follower VM, install NRP Container in Manager VM
For example: To build the image for nrp-core, we can use command "docker build -t nrp:v0 ." inside the floder "NRP". "nrp:v0" is the name and tag of the docker image.


Step 3: Config the  Follower  VM


Change “/usr/lib/systemd/system/docker.service”, we can find one sentence “ExecStart = …” in this file. Add “-H tcp://0.0.0.0:2375” at the end of this sentence.
	

Open the port to firewall
	
	sudo apt-get install firewalld
	
	sudo firewall-cmd --add-port=2377/tcp --permanent
	
	sudo firewall-cmd –reload execu

To execute the example:

Open all Manager VM and follower VM.

In the Manager VM, use the docker images to run docker container. 

sudo docker run –name nrpnode -it nrp:v0

Inside the docker container, go to nrp-core/example/net_opensim, and run

	NRPCoreSim -c simulation_config.json -p "NRPPySimEngine.so"

And the simulation will be run with NRP

Pay attention, the IP address of different VM may be different. If it connect fail when nrp working, it may be need to change the ip address. There two location to set the ip address.

In nrp_pysim_engines/nrp_pysim_engine/python/NetConnector.py,
	self.client = DockerClient(base_url='tcp://10.0.2.15:2375') 
in the initialization of the class. The base_url is “tcp://IP:port” for the Follower VM

In examples/net_opensim/simulation_config.json, there are properties “IP” and “Port” in the configure json. And the IP address need to be the same with then  docker container running in the Follower VM.

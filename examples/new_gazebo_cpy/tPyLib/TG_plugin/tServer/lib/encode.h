#ifndef ENCODE_H
#define ENCODE_H

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include <gazebo/gazebo.hh>
#include <gazebo/gazebo_config.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

#include <iostream>

char const* hello();

void RunServer(gazebo::Server *server, std::string worldName);
class GZ_Server{
public:
	GZ_Server();
	std::string value = "aloha";
	void setWorld(std::string worldFile);
	void loadWorld(std::string worldFile);

	void shutdown();
private:
	std::string worldName = "";
	gazebo::Server *tServer;
};
#endif
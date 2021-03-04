#ifndef ENCODE_H
#define ENCODE_H

#include <gazebo/gazebo.hh>
#include <gazebo/gazebo_client.hh>
#include <gazebo/gazebo_config.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

#include <iostream>

char const* hello();

class GZ_Client{
public:
	GZ_Client();
	std::string value = "aloha";
	void setListener(std::string topicName, std::string msgType);
	void toListen();
	std::string getMsg();
	double getSimTime();
	void setMsg(std::string data);

	void shutdown();
private:
	void cb(ConstWorldStatisticsPtr &_msg);

	gazebo::transport::NodePtr node;
	gazebo::transport::SubscriberPtr sub_1;
	std::string tMsg;
	double _sim_time;

};
#endif
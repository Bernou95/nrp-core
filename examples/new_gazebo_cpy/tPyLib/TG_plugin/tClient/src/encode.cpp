#include "encode.h"
#include <iostream>
#include <math.h>
using namespace std;

char const* hello(){
  return "hello, world";
}

GZ_Client::GZ_Client(){
	//bool res = tServer->LoadFile();
	gazebo::client::setup();
	gazebo::transport::NodePtr tNode(new gazebo::transport::Node());
	this->node = tNode;
	this->node->Init();
}

std::string GZ_Client::getMsg(){
	return this->tMsg;
}
double GZ_Client::getSimTime(){
	return this->_sim_time;
}

void GZ_Client::cb(ConstWorldStatisticsPtr &_msg){
	gazebo::msgs::Time tt = _msg->sim_time();
	this->tMsg = _msg->DebugString();
	this->_sim_time = (double)tt.sec() + (double)tt.nsec()/pow(10, 9);
	//this->_sim_time = tt.nsec();
}
void GZ_Client::setListener(std::string topicName, std::string msgType){
	//bool res = tServer->LoadFile();
	//gazebo::transport::SubscriberPtr sub = 
	//	node->Subscribe(topicName, &GZ_Client::cb, this);
	this->sub_1 = node->Subscribe(topicName, &GZ_Client::cb, this);
	//for(int i =0; i < 10; i++)
  	//	gazebo::common::Time::MSleep(10);
}

void GZ_Client::toListen(){
	gazebo::common::Time::MSleep(10);
}

void GZ_Client::shutdown(){
	gazebo::client::shutdown();
}


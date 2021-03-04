#include "encode.h"
#include <iostream>

using namespace std;

char const* hello(){
  return "hello, world";
}

GZ_Server::GZ_Server(){
  boost::scoped_ptr<gazebo::Server> server(new gazebo::Server());
	this->tServer = new gazebo::Server();
	//bool res = tServer->LoadFile();
}

void GZ_Server::setWorld(std::string worldFile){
	this->worldName = worldFile;
}
void GZ_Server::loadWorld(std::string worldFile){
  boost::thread serverThread(RunServer, this->tServer, worldFile);
  //RunServer();
}
void GZ_Server::shutdown(){
  this->tServer->Stop();
  gazebo::shutdown();
}


void RunServer(gazebo::Server *server, std::string worldName){
  // Initialize gazebo server.
  try
  {
    if (!server->ParseArgs(0, NULL))
      return;
    // Initialize the informational logger. This will log warnings, and errors.
    gzLogInit("server-", "gzserver.log");
    server->LoadFile(worldName);
    server->Run();
    server->Fini();
  }
  catch(gazebo::common::Exception &_e)
  {
    _e.Print();
    server->Fini();
  }
}


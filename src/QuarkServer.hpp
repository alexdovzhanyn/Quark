#pragma once 
#include <iostream>
#include <memory>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <thread>
#include <arpa/inet.h>
#include <sys/types.h>
#include "ConnectionHandler.hpp"

using namespace std;

struct FreeAddrinfo {
  void operator()(addrinfo* ptr) const {
    if (ptr) freeaddrinfo(ptr);
  }
};

class QuarkServer {
public:
  string serverAddress;
  string ipVersion = "IPv6";
  string port;
  int socketDescriptor;
  QuarkServer(string serverPort);

  void run();

private:
  bool startupSuccess = false;

  unique_ptr<addrinfo, FreeAddrinfo> getServerInfo();
};

#pragma once 
#include <functional>
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
#include <vector>
#include "ConnectionHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

using namespace std;

struct FreeAddrinfo {
  void operator()(addrinfo* ptr) const {
    if (ptr) freeaddrinfo(ptr);
  }
};

namespace Quark {
  class Server {
  public:
    string serverAddress;
    string ipVersion = "IPv6";
    string port;
    int socketDescriptor;
    Server(string serverPort);
  
    void staticServe(const std::string &path);

    Server& registerRequestMiddleware(std::function<void(HttpRequest&)> middlewareHandler);
    Server& registerResponseMiddleware(std::function<void(HttpResponse&)> middlewareHandler);

    void run();

  private:
    bool startupSuccess = false;

    unique_ptr<addrinfo, FreeAddrinfo> getServerInfo();

    vector<std::function<void(HttpRequest&)>> requestMiddlewares;
    vector<std::function<void(HttpResponse&)>> responseMiddlewares;
  };
}


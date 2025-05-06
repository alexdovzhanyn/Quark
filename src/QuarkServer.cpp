#include "QuarkServer.hpp"

unique_ptr<addrinfo, FreeAddrinfo> Quark::Server::getServerInfo() {
  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  string PORT = "9669";

  addrinfo* rawServInfo = nullptr;
  int status = getaddrinfo(nullptr, port.c_str(), &hints, &rawServInfo);

  if (status != 0) {
    cerr << "Error initializing server: " << gai_strerror(status) << endl;
    exit(1);
  }

  return unique_ptr<addrinfo, FreeAddrinfo>(rawServInfo);
}

Quark::Server::Server(string serverPort) : port(serverPort) {
  unique_ptr<addrinfo, FreeAddrinfo> serverInfo = getServerInfo();  
  void* addr;

  if (serverInfo->ai_family == AF_INET) {
    sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(serverInfo->ai_addr);
    addr = &(ipv4->sin_addr);
    ipVersion = "IPv4";
  } else {
    sockaddr_in6* ipv6 = reinterpret_cast<sockaddr_in6*>(serverInfo->ai_addr);
    addr = &(ipv6->sin6_addr);
  }

  char ipstr[INET6_ADDRSTRLEN];
  inet_ntop(serverInfo->ai_family, addr, ipstr, sizeof ipstr);

  serverAddress = ipstr;

  cout << "Attempting to bind to " << ipVersion << " socket on address " << serverAddress << ":" << port << "..." << endl;
 
  socketDescriptor = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

  if (socketDescriptor < 0) {
    cerr << "Error provisioning socket: " << strerror(errno) << endl;
  }

  if (::bind(socketDescriptor, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
    cerr << "Error binding to socket: " << strerror(errno) << endl;
    close(socketDescriptor);
    exit(1);
  } 
}

void Quark::Server::run() {
  if (listen(socketDescriptor, 10) < 0) {
    cerr << "Failed to listen on port " << port << "! " << strerror(errno) << endl;
    close(socketDescriptor);
    exit(1);
  }

  if (!startupSuccess) {
    cout << "Listening for connections at " << serverAddress << ":" << port << " [" << ipVersion << "]" << endl << endl; 
    startupSuccess = true;
  }

  while (true) {
    sockaddr_storage incomingAddr;
    socklen_t addr_size = sizeof incomingAddr;

    int incomingSocketDescriptor = accept(socketDescriptor, reinterpret_cast<sockaddr*>(&incomingAddr), &addr_size);

    if (incomingSocketDescriptor > 0) {
      shared_ptr<ConnectionHandler> handler = make_shared<ConnectionHandler>(incomingSocketDescriptor, incomingAddr);

      thread handlerThread(&ConnectionHandler::handleHttpRequest, handler);
      handlerThread.detach();
    } 
  }
}


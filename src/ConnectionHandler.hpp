#include "HttpRequest.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

class ConnectionHandler {
public:
  ConnectionHandler(int sock, sockaddr_storage addr) : socketDescriptor(sock), connectionAddress(addr) {}

  void handleHttpRequest();

private:
  int socketDescriptor;
  sockaddr_storage connectionAddress;

  void logRequest(
    const std::chrono::time_point<std::chrono::steady_clock> &requestStart,
    const std::chrono::time_point<std::chrono::steady_clock> &requestEnd,
    const HttpRequest &request,
    const int &statusCode
  );

  std::string getIpFromSocket(); 
};

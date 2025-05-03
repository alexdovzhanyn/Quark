#include <arpa/inet.h>
#include <chrono>
#include <iomanip>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"

class ConnectionHandler {
public:
  ConnectionHandler(int sock, sockaddr_storage addr) : socketDescriptor(sock), connectionAddress(addr) {}

  void handleHttpRequest() {
    std::chrono::time_point requestStart = std::chrono::high_resolution_clock::now();

    HttpRequest request = HttpRequest(getIpFromSocket());

    HttpRequestParser parser = HttpRequestParser(socketDescriptor, request);
    if (parser.parseIncomingRequest()) {
      std::time_t localTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

      std::ostringstream log;
      log << "[" << std::put_time(std::localtime(&localTime), "%Y-%m-%d %H:%M:%S") << "] "
        << request.method << " " << request.path << request.queryString
        << " " << request.protocolVersion;
      log << " <" << request.ip << "> ";

      std::chrono::time_point requestEnd = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> duration = requestEnd - requestStart;
      log << "Took " << duration.count() << "ms";

      std::cout << log.str() << std::endl;
    }

    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
    send(socketDescriptor, response, strlen(response), 0);
    close(socketDescriptor);
  }

private:
  int socketDescriptor;
  sockaddr_storage connectionAddress;

  std::string getIpFromSocket() {
    char ip[INET6_ADDRSTRLEN];

    if (connectionAddress.ss_family == AF_INET) {
      const sockaddr_in *ipv4 = reinterpret_cast<const sockaddr_in*>(&connectionAddress);
      inet_ntop(AF_INET, &(ipv4->sin_addr), ip, sizeof(ip));
    } else {
      const sockaddr_in6 *ipv6 = reinterpret_cast<const sockaddr_in6*>(&connectionAddress);
      inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, sizeof(ip));
    }

    return std::string(ip);
  }
};

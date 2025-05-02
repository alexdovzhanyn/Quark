#include <chrono>
#include <iomanip>
#include <sstream>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include "HttpRequest.hpp"

class ConnectionHandler {
public:
  ConnectionHandler(int sock, sockaddr_storage addr) : socketDescriptor(sock), connectionAddress(addr) {}

  void handleHttpRequest() {
    std::time_t requestStart = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char buffer[RECV_BUFFER_SIZE];
    ssize_t bytesReceived;

    std::string request;
    int parsePointer = 0;
    int receivedBodyBytes = 0;

    HttpRequest httpRequest = HttpRequest();

    // Some clients seem to make extra, empty dummy requests for some reason
    bool isRealRequest = false;
    while ((bytesReceived = recv(socketDescriptor, &buffer, RECV_BUFFER_SIZE, 0)) > 0) {
      isRealRequest = true;

      request.append(buffer, bytesReceived);

      if (!httpRequest.parsedStartLine) {
        // We haven't even received all the start-line data yet. Wait until we have some more.
        if (request.find("\r\n") == std::string::npos) continue;

        httpRequest.parseStartLine(request, parsePointer);
      }

      if (!httpRequest.parsedHeaders) {
        // We haven't finished receiving the headers yet
        if (request.find("\r\n\r\n") == std::string::npos) continue;

        httpRequest.parseHeaders(request, parsePointer);
      }

      if (httpRequest.headers.find("Content-Length") == httpRequest.headers.end()) break;
  
      // TODO: do something with body
      if (stoi(httpRequest.headers.at("Content-Length")) == request.length() - parsePointer) break;
    }

    if (isRealRequest) {
      std::ostringstream log;
      log << "[" << std::put_time(std::localtime(&requestStart), "%Y-%m-%d %H:%M:%S") << "] "
        << httpRequest.method << " " << httpRequest.path;

      std::cout << log.str() << std::endl;
    }

    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
    send(socketDescriptor, response, strlen(response), 0);
    close(socketDescriptor);
  }

private:
  int socketDescriptor;
  sockaddr_storage connectionAddress;
  int RECV_BUFFER_SIZE = 8192;
};

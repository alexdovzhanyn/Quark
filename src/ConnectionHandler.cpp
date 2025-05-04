#include "ConnectionHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponseBuilder.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "HttpException.hpp"

void ConnectionHandler::handleHttpRequest() {
  std::chrono::time_point requestStart = std::chrono::high_resolution_clock::now();

  HttpRequest request = HttpRequest(getIpFromSocket());
  HttpRequestParser parser = HttpRequestParser(socketDescriptor, request);
  HttpResponseBuilder responseBuilder = HttpResponseBuilder();

  bool realRequest = false;
  try {
    realRequest = parser.parseIncomingRequest();
  } catch (const HttpException &e) {
    std::cout << "OH NO SOME ERROR!" << std::endl;
    responseBuilder.setStatus(e.code, e.what()); 
    std::string response = responseBuilder.str();

    std::cout << "RESPONDING WITH:" << std::endl;
    std::cout << response << std::endl;
    send(socketDescriptor, response.c_str(), response.length(), 0);

    logRequest(requestStart, std::chrono::high_resolution_clock::now(), request, e.code);
    return;
  }

  responseBuilder.setStatus(200);
  std::string response = responseBuilder.str();
  send(socketDescriptor, response.c_str(), response.length(), 0);

  if (realRequest) logRequest(requestStart, std::chrono::high_resolution_clock::now(), request, 200);
  close(socketDescriptor);
}

void ConnectionHandler::logRequest(
  const std::chrono::time_point<std::chrono::steady_clock> &requestStart,
  const std::chrono::time_point<std::chrono::steady_clock> &requestEnd,
  const HttpRequest &request,
  const int &statusCode
) {
  std::time_t localTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::ostringstream log;
  log << "[" << std::put_time(std::localtime(&localTime), "%Y-%m-%d %H:%M:%S") << "] "
    << request.method << " " << request.path << request.queryString
    << " " << request.protocolVersion;
  log << " <" << request.ip << "> ";
  log << std::to_string(statusCode) << " ";

  std::chrono::duration<double, std::milli> duration = requestEnd - requestStart;
  log << "Took " << duration.count() << "ms";

  std::cout << log.str() << std::endl;
}

std::string ConnectionHandler::getIpFromSocket() {
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

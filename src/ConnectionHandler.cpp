#include "ConnectionHandler.hpp"
#include "QuarkRouter.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "HttpException.hpp"

void Quark::ConnectionHandler::handleHttpRequest() {
  std::chrono::time_point requestStart = std::chrono::high_resolution_clock::now();

  HttpRequest request = HttpRequest(getIpFromSocket());
  HttpRequestParser parser = HttpRequestParser(socketDescriptor, request);

  bool realRequest = false;
  try {
    realRequest = parser.parseIncomingRequest();
  } catch (const HttpException &e) {
    HttpResponse response = HttpResponse(e.code, e.what());
    std::string responseStr = response.str();

    send(socketDescriptor, responseStr.c_str(), responseStr.length(), 0);

    logRequest(requestStart, std::chrono::high_resolution_clock::now(), request, e.code);
    return;
  }

  applyRequestMiddleware(request);
  HttpResponse response = Router::getInstance().routeRequest(request);
  applyResponseMiddleware(response);

  std::string responseStr = response.str();
  send(socketDescriptor, responseStr.c_str(), responseStr.length(), 0);

  if (realRequest) logRequest(requestStart, std::chrono::high_resolution_clock::now(), request, response.statusCode);
  close(socketDescriptor);
}

void Quark::ConnectionHandler::applyRequestMiddleware(HttpRequest &request) {
  for (auto middleware : requestMiddlewares) {
    middleware(request);
  }
} 

void Quark::ConnectionHandler::applyResponseMiddleware(HttpResponse &response) {
  for (auto middleware : responseMiddlewares) {
    middleware(response);
  }
}

void Quark::ConnectionHandler::logRequest(
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

std::string Quark::ConnectionHandler::getIpFromSocket() {
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

#include "ConnectionHandler.hpp"
#include "QuarkRouter.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <sys/_types/_fd_def.h>
#include "HttpException.hpp"

void Quark::ConnectionHandler::handleHttpRequest() {
  while (socketDescriptor != -1) {
    HttpRequest request = HttpRequest(getIpFromSocket());
    HttpRequestParser parser = HttpRequestParser(socketDescriptor, request);

    if (awaitActivity() <= 0) return closeConnection();

    bool realRequest = false;
    try {
      realRequest = parser.parseIncomingRequest(MAX_REQUEST_TIMEOUT);
    } catch (const HttpException &e) {
      HttpResponse response = HttpResponse(e.code, e.what());
      std::string responseStr = response.str();

      send(socketDescriptor, responseStr.c_str(), responseStr.length(), 0);

      logRequest(std::chrono::high_resolution_clock::now(), request, e.code);

      // Always close connections if we encounter an error
      return closeConnection();
    }

    applyRequestMiddleware(request);
    HttpResponse response = Router::getInstance().routeRequest(request);
    applyResponseMiddleware(response);
    requestsProcessed++;

    // Default to keep-alive as per http/1.1 spec
    bool isKeepAlive = request.headers.find("Connection") == request.headers.end() || request.headers.at("Connection") == "keep-alive";

    if (isKeepAlive) {
      std::string keepAliveHeader = "timeout=" + std::to_string(MAX_REQUEST_TIMEOUT) + ", max=" + std::to_string(MAX_KEEPALIVE_REQUESTS);
      response.addHeader("Connection", "keep-alive");
      response.addHeader("Keep-Alive", keepAliveHeader);
    }

    std::string responseStr = response.str();
    send(socketDescriptor, responseStr.c_str(), responseStr.length(), 0);

    if (realRequest) logRequest(std::chrono::high_resolution_clock::now(), request, response.statusCode);

    if (!realRequest || !isKeepAlive || requestsProcessed >= MAX_KEEPALIVE_REQUESTS) return closeConnection();
  }
}

int Quark::ConnectionHandler::awaitActivity() {
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(socketDescriptor, &readfds);
  timeval tv;
  tv.tv_sec = MAX_REQUEST_TIMEOUT;
  tv.tv_usec = 0;

  return select(socketDescriptor + 1, &readfds, nullptr, nullptr, &tv);
}

void Quark::ConnectionHandler::closeConnection() {
  if (socketDescriptor < 0) return;

  close(socketDescriptor);
  socketDescriptor = -1;
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

  std::chrono::duration<double, std::milli> duration = requestEnd - request.requestStart.value();
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

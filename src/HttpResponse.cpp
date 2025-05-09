#include "HttpResponse.hpp"
#include <sstream>
#include <iostream>

Quark::HttpResponse& Quark::HttpResponse::setStatus(int status, std::string message) {
  statusCode = status;
  statusMessage = message;

  return *this;
}

Quark::HttpResponse& Quark::HttpResponse::setBody(std::string bod) {
  body = bod;
  addHeader("Content-Length", std::to_string(body.length()));
  return *this;
}

Quark::HttpResponse& Quark::HttpResponse::addHeader(const std::string &headerName, const std::string &headerValue) {
  headers[headerName] = headerValue;
  return *this;
}

Quark::HttpResponse Quark::HttpResponse::ok(const std::string &body) {
  HttpResponse response = HttpResponse(200, "OK");

  response.setBody(body);

  return response;
}

Quark::HttpResponse Quark::HttpResponse::notFound() {
  return HttpResponse(404, "Not Found");
}

std::string Quark::HttpResponse::str() {
  std::ostringstream oss;

  // Response Line
  oss << protocolVersion << " " << std::to_string(statusCode) << " " << statusMessage << "\r\n";

  // Headers
  for (auto it : headers) {
    oss << it.first << ": " << it.second << "\r\n";
  }

  oss << "\r\n"; // End header section
  oss << body;
  
  return oss.str();
}

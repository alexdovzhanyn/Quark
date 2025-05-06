#include "HttpResponse.hpp"
#include <sstream>
#include <iostream>

void Quark::HttpResponse::setStatus(int status, std::string message) {
  statusCode = status;
  statusMessage = message;
}

void Quark::HttpResponse::setBody(std::string bod) {
  body = bod;
}

void Quark::HttpResponse::addHeader(const std::string &headerName, const std::string &headerValue) {
  headers.insert({ headerName, headerValue });
}

std::string Quark::HttpResponse::str() {
  std::ostringstream oss;

  // Response Line
  oss << protocolVersion << " " << std::to_string(statusCode) << " " << statusMessage << "\r\n";

  // Headers
  oss << "Content-Length: " << body.length() << "\r\n";

  for (auto it : headers) {
    oss << it.first << ": " << it.second << "\r\n";
  }

  oss << "\r\n"; // End header section
  oss << body;
  
  return oss.str();
}

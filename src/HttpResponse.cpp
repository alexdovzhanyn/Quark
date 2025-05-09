#include "HttpResponse.hpp"
#include "util/FileHelper.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

Quark::HttpResponse& Quark::HttpResponse::setStatus(int status, std::string message) {
  statusCode = status;
  statusMessage = message;

  return *this;
}

Quark::HttpResponse& Quark::HttpResponse::setBody(std::string bod, bool updateContentLength) {
  body = bod;

  if (updateContentLength) addHeader("Content-Length", std::to_string(body.length()));

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

Quark::HttpResponse Quark::HttpResponse::internalServerError() {
  return HttpResponse(500, "Internal server error");
}

Quark::HttpResponse Quark::HttpResponse::sendFile(const std::string &filePath) {
  std::stringstream buffer;
  std::ifstream file(filePath);

  if (!file.is_open()) throw std::runtime_error("Error opening file");

  buffer << file.rdbuf();

  std::string mimeType = FileHelper::getMimeType(filePath);

  return ok()
    .addHeader("Content-Type", mimeType)
    .setBody(buffer.str());
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

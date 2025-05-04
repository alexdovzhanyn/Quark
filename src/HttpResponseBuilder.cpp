#include "HttpResponseBuilder.hpp"
#include <sstream>

void HttpResponseBuilder::setStatus(int status, std::string message) {
  statusCode = status;
  statusMessage = message;
}

void HttpResponseBuilder::setBody(std::string bod) {

}

std::string HttpResponseBuilder::str() {
  std::ostringstream oss;

  // Response Line
  oss << protocolVersion << " " << std::to_string(statusCode) << " " << statusMessage << "\r\n";

  // Headers
  oss << "Content-Length: " << responseBody.length();

  oss << "\r\n\r\n"; // End header section

  return oss.str();
}

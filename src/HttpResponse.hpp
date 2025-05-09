#pragma once
#include <string>
#include <unordered_map>

namespace Quark {
  class HttpResponse {
  public:
    int statusCode;

    HttpResponse() = default;
    HttpResponse(int status, std::string message) : statusCode(status), statusMessage(message) {}

    static HttpResponse ok(const std::string &body = "");
    static HttpResponse notFound();
    
    HttpResponse& setStatus(int status, std::string message = "");
    HttpResponse& setBody(std::string bod);
    HttpResponse& addHeader(const std::string &headerName, const std::string &headerValue);

    std::string str();
  private:
    std::string protocolVersion = "HTTP/1.1";
    std::string statusMessage;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
  };
}

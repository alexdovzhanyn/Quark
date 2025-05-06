#pragma once

#include <exception>
#include <string>

namespace Quark {
  class HttpException : public std::exception {
  public:
    int code;
    std::string message;

    HttpException(int code, const std::string &msg) : code(code), message(msg) {}

    const char* what() const noexcept override {
      return message.c_str();
    }
  };

  class MethodNotImplemented : public HttpException {
  public:
    MethodNotImplemented() : HttpException(405, "Method Not Allowed") {}
  };

  class MalformedRequest : public HttpException {
  public:
    MalformedRequest() : HttpException(400, "Bad Request") {}
  };

  class VersionNotSupported : public HttpException {
  public:
    VersionNotSupported() : HttpException(501, "HTTP Version Not Supported") {}
  };
}

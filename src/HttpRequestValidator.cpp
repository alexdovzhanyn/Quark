#include "HttpRequestValidator.hpp"
#include "HttpException.hpp"

const std::unordered_map<std::string, Quark::RequestMethod> Quark::HttpRequestValidator::methodMapping = {
  { "GET", RequestMethod::GET },
  { "PATCH", RequestMethod::PATCH },
  { "POST", RequestMethod::POST },
  { "PUT", RequestMethod::PUT },
  { "DELETE", RequestMethod::DELETE },
  { "OPTIONS", RequestMethod::OPTIONS },
  { "HEAD", RequestMethod::HEAD }
};

void Quark::HttpRequestValidator::validateMethod(std::string method) {
  // Will throw if invalid method name
  methodStringToEnum(method);
}

void Quark::HttpRequestValidator::validateProtocolVersion(std::string version) {
  if (version != "HTTP/1.1") throw VersionNotSupported();
}

Quark::RequestMethod Quark::HttpRequestValidator::methodStringToEnum(std::string method) {
  if (methodMapping.find(method) == methodMapping.end()) throw MethodNotImplemented();

  return methodMapping.at(method);
}


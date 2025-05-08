#pragma once
#include <string>

namespace Quark {
  enum RequestMethod {
    GET,
    POST,
    PUT,
    DELETE,
    OPTIONS,
    HEAD
  };

  std::string requestMethodToString(RequestMethod method);
}

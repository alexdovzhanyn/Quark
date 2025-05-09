#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class UserController {
public:
  static Quark::HttpResponse createUser(Quark::HttpRequest &request) {
    return Quark::HttpResponse::ok()
      .setBody(R"({
        "name": "Mike Wazowski",
        "age": 38
      })")
      .addHeader("Content-Type", "text/json");
  }
};

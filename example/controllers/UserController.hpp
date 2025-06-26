#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "../models/User.hpp"

class UserController {
public:
  static Quark::HttpResponse createUser(Quark::HttpRequest &request) {
    User user = User("Mike Wazowski", 25, "mike.wazowski@monstersinc.com");

    return Quark::HttpResponse::ok().json(user);
  }

  static Quark::HttpResponse updateUser(Quark::HttpRequest &request) {
    return Quark::HttpResponse::ok()
      .addHeader("Content-Type", "text/json")
      .setBody(R"({
        "name": "Mike Wazowski",
        "age": 38
      })");
  }
};

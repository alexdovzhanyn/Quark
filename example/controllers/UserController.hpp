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
};

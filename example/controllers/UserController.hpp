#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class UserController {
public:
  static void createUser(Quark::HttpRequest &request, Quark::HttpResponse &response) {
    response.setStatus(201);
    response.addHeader("Content-Type", "text/json");
    response.setBody(R"({
      "name": "Mike Wazowski",
      "age": 38
    })");
  }

  static void updateUser(Quark::HttpRequest &request, Quark::HttpResponse &response) {
    response.setStatus(200);
    response.addHeader("Content-Type", "text/json");
    response.setBody(R"({
      "name": "Mike Wazowski",
      "age": 38
    })");
  }
};

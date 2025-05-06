#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class ApplicationController {
public:
  static void getRoot(Quark::HttpRequest &request, Quark::HttpResponse &response) {
    response.setStatus(200);
    response.addHeader("Content-Type", "text/json");
    response.setBody(R"({
      "location": "root"
    })");
  }

  static void helloWorld(Quark::HttpRequest &request, Quark::HttpResponse &response) {
    response.setStatus(200);
    response.addHeader("Content-Type", "text/json");
    response.setBody(R"({
      "hello": "world"
    })");
  }
};

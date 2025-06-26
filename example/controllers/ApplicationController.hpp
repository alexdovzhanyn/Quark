#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class ApplicationController {
public:
  static Quark::HttpResponse getRoot(Quark::HttpRequest &request) {
    return Quark::HttpResponse::sendFile("public/index.html");
  }

  static Quark::HttpResponse helloWorld(Quark::HttpRequest &request) {
    Quark::HttpResponse response = Quark::HttpResponse();

    response
      .setStatus(200)
      .addHeader("Content-Type", "text/json")
      .setBody(R"({
        "hello": "world"
      })");

    return response;
  }

  static Quark::HttpResponse pathParamTest(Quark::HttpRequest &request) {
    return Quark::HttpResponse::ok();
  }
};

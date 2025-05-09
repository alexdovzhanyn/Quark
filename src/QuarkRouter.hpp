#pragma once
#include "HttpRequest.hpp"
#include "RequestMethod.hpp"
#include "HttpResponse.hpp"
#include <unordered_map>
#include <functional>

namespace Quark {
  class Router {
  public:
    using RouteHandler = std::function<HttpResponse(HttpRequest&)>;
    using RouteTable = std::unordered_map<RequestMethod, RouteHandler>;

    static Router& getInstance() {
      static Router instance;
      return instance;
    }

    static void GET(std::string route, RouteHandler handler); 

    static void POST(std::string route, RouteHandler handler); 

    static void PUT(std::string route, RouteHandler handler); 

    static void DELETE(std::string route, RouteHandler handler);

    HttpResponse routeRequest(HttpRequest& request); 

    static void setStaticServePath(const std::string &path);

  private:
    std::unordered_map<std::string, RouteTable> routeMapping;
    std::string staticServePath = "public";

    Router() = default;

    Router(const Router&) = delete;
    Router& operator=(const Router&) = delete;

    static void addRoute(const RequestMethod &method, const std::string &route, RouteHandler handler);

    bool isValidRoute(const std::string &path);

    HttpResponse handleRequest(HttpRequest &request);

    HttpResponse handleOptionsRequest(std::string &path);

    HttpResponse handleHeadRequest(std::string &path, HttpRequest &request); 
  };
}


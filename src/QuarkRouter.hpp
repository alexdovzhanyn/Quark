#pragma once
#include "HttpRequest.hpp"
#include "RequestMethod.hpp"
#include "HttpResponse.hpp"
#include <unordered_map>

namespace Quark {
  class Router {
  public:
    using RouteHandler = std::function<void(HttpRequest&, HttpResponse&)>;
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

  private:
    std::unordered_map<std::string, RouteTable> routeMapping;

    Router() = default;

    Router(const Router&) = delete;
    Router& operator=(const Router&) = delete;

    static void addRoute(const RequestMethod &method, const std::string &route, RouteHandler handler);

    void handleOptionsRequest(std::string &path, HttpResponse &response);

    void handleHeadRequest(std::string &path, HttpRequest &request, HttpResponse &response); 
  };
}


#pragma once
#include "HttpRequest.hpp"
#include "RequestMethod.hpp"
#include "HttpResponse.hpp"
#include "RouteNode.hpp"
#include <memory>
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
    std::unordered_map<std::string, RouteTable> simpleRoutes;
    std::unordered_map<std::shared_ptr<RouteNode>, RouteTable> complexRoutes;


    std::string staticServePath = "public";

    Router() = default;

    Router(const Router&) = delete;
    Router& operator=(const Router&) = delete;

    static void addRoute(const RequestMethod &method, const std::string &route, RouteHandler handler);

    static std::shared_ptr<RouteNode> parsePathParams(const std::string &route);

    std::shared_ptr<RouteNode> findMatchingRoute(const std::string &path);
    std::optional<RouteTable*> findRouteTableForRoute(const std::string &path); 

    bool isValidRoute(const std::string &path);

    HttpResponse handleRequest(HttpRequest &request);

    HttpResponse handleOptionsRequest(std::string &path);

    HttpResponse handleHeadRequest(std::string &path, HttpRequest &request); 
  };
}


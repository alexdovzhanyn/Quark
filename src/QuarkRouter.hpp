#pragma once
#include "HttpRequest.hpp"
#include "RequestMethod.hpp"
#include "HttpResponse.hpp"
#include <unordered_map>

namespace Quark {
  class Router {
  public:
    using RouteHandler = std::function<void(HttpRequest&, HttpResponse&)>;
    using RouteTable = std::unordered_map<std::string, RouteHandler>;

    static Router& getInstance() {
      static Router instance;
      return instance;
    }

    void get(std::string route, RouteHandler handler) {
      addRoute(RequestMethod::GET, route, handler);
    } 

    void post(std::string route, RouteHandler handler) {
      addRoute(RequestMethod::POST, route, handler);
    }

    HttpResponse routeRequest(HttpRequest& request) {
      HttpResponse response = HttpResponse();

      RequestMethod requestMethod = request.getRequestMethod();
      std::string requestPath = request.path;

      if (
        routeMapping.find(requestMethod) == routeMapping.end() || 
        routeMapping[requestMethod].find(requestPath) == routeMapping[requestMethod].end()
      ) {
        response.setStatus(404, "Not Found");
      } else {
        routeMapping[request.getRequestMethod()][request.path](request, response);   
      }

      return response;
    }
  private:
    std::unordered_map<RequestMethod, RouteTable> routeMapping;

    Router() = default;

    Router(const Router&) = delete;
    Router& operator=(const Router&) = delete;

    void addRoute(RequestMethod method, const std::string &route, RouteHandler handler) {
      routeMapping[method][route] = std::move(handler);
    }
  };
}


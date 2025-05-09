#pragma once
#include "HttpRequest.hpp"
#include "RequestMethod.hpp"
#include "HttpResponse.hpp"
#include <strstream>
#include <unordered_map>
#include <functional>

namespace Quark {
  class Router {
  public:
    using RouteHandler = std::function<void(HttpRequest&, HttpResponse&)>;
    using RouteTable = std::unordered_map<RequestMethod, RouteHandler>;

    static Router& getInstance() {
      static Router instance;
      return instance;
    }

    static void GET(std::string route, RouteHandler handler) {
      addRoute(RequestMethod::GET, route, handler);
    } 

    static void POST(std::string route, RouteHandler handler) {
      addRoute(RequestMethod::POST, route, handler);
    }

    static void PUT(std::string route, RouteHandler handler) {
      addRoute(RequestMethod::PUT, route, handler);
    }

    static void DELETE(std::string route, RouteHandler handler) {
      addRoute(RequestMethod::DELETE, route, handler);
    }

    HttpResponse routeRequest(HttpRequest& request) {
      HttpResponse response = HttpResponse();

      RequestMethod requestMethod = request.getRequestMethod();
      std::string requestPath = request.path;

      if (routeMapping.find(requestPath) == routeMapping.end()) {
        response.setStatus(404, "Not Found");
        return response;
      }

      if (requestMethod == RequestMethod::OPTIONS) {
        handleOptionsRequest(requestPath, response);
        return response;
      }

      if (requestMethod == RequestMethod::HEAD) {
        handleHeadRequest(requestPath, request, response); 
        return response;
      }

      if (routeMapping[requestPath].find(requestMethod) == routeMapping[requestPath].end()) {
        response.setStatus(404, "Not Found");
      } else {
        routeMapping[request.path][request.getRequestMethod()](request, response);   
      }

      return response;
    }
  private:
    std::unordered_map<std::string, RouteTable> routeMapping;

    Router() = default;

    Router(const Router&) = delete;
    Router& operator=(const Router&) = delete;

    static void addRoute(const RequestMethod &method, const std::string &route, RouteHandler handler) {
      Router& router = getInstance();
      router.routeMapping[route][method] = std::move(handler);
    }

    void handleOptionsRequest(std::string &path, HttpResponse &response) {
      response.setStatus(200, "OK");

      std::ostrstream oss;
    
      int i = 0;
      for (auto it : routeMapping[path]) {
        if (i > 0) oss << ", ";

        oss << requestMethodToString(it.first); 
      }

      oss << ", OPTIONS";

      if (routeMapping[path].find(RequestMethod::GET) != routeMapping[path].end()) {
        oss << ", HEAD";
      }

      response.addHeader("Allow", oss.str());
    } 

    void handleHeadRequest(std::string &path, HttpRequest &request, HttpResponse &response) {
      if (routeMapping[path].find(RequestMethod::GET) == routeMapping[path].end()) {
        response.setStatus(404, "Not Found");
        return;
      }

      routeMapping[path][RequestMethod::GET](request, response);
      response.setBody("");
    }
  };
}


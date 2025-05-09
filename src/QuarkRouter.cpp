#include "QuarkRouter.hpp"
#include <strstream>

void Quark::Router::GET(std::string route, RouteHandler handler) {
  addRoute(RequestMethod::GET, route, handler);
}

void Quark::Router::POST(std::string route, RouteHandler handler) {
  addRoute(RequestMethod::POST, route, handler);
}

void Quark::Router::PUT(std::string route, RouteHandler handler) {
  addRoute(RequestMethod::PUT, route, handler);
}

void Quark::Router::DELETE(std::string route, RouteHandler handler) {
  addRoute(RequestMethod::DELETE, route, handler);
}

Quark::HttpResponse Quark::Router::routeRequest(HttpRequest& request) {
  RequestMethod requestMethod = request.getRequestMethod();
  std::string requestPath = request.path;

  if (routeMapping.find(requestPath) == routeMapping.end()) return HttpResponse::notFound();

  if (requestMethod == RequestMethod::OPTIONS) return handleOptionsRequest(requestPath);

  if (requestMethod == RequestMethod::HEAD) return handleHeadRequest(requestPath, request); 

  if (routeMapping[requestPath].find(requestMethod) == routeMapping[requestPath].end()) return HttpResponse::notFound();
    
  return routeMapping[request.path][request.getRequestMethod()](request);   
}

void Quark::Router::addRoute(const RequestMethod &method, const std::string &route, RouteHandler handler) {
  Router& router = getInstance();
  router.routeMapping[route][method] = std::move(handler);
}

Quark::HttpResponse Quark::Router::handleOptionsRequest(std::string &path) {
  HttpResponse response = HttpResponse::ok();

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

  return response.addHeader("Allow", oss.str());
} 

Quark::HttpResponse Quark::Router::handleHeadRequest(std::string &path, HttpRequest &request) {
  if (routeMapping[path].find(RequestMethod::GET) == routeMapping[path].end()) return HttpResponse::notFound();

  return routeMapping[path][RequestMethod::GET](request).setBody("");
}

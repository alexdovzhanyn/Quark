#include "QuarkRouter.hpp"
#include "RequestMethod.hpp"
#include "util/FileHelper.hpp"
#include <stdexcept>
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

void Quark::Router::setStaticServePath(const std::string &path) {
  Router& router = getInstance(); 
  router.staticServePath = path;
}

Quark::HttpResponse Quark::Router::routeRequest(HttpRequest& request) {
  RequestMethod requestMethod = request.getRequestMethod();
  std::string requestPath = request.path;

  if (requestMethod == RequestMethod::OPTIONS) return handleOptionsRequest(requestPath);
  if (requestMethod == RequestMethod::HEAD) return handleHeadRequest(requestPath, request); 

  if (!isValidRoute(requestPath)) return HttpResponse::notFound();

  try {
    return handleRequest(request);
  } catch (std::exception e) {
    return HttpResponse::internalServerError();
  }
}

void Quark::Router::addRoute(const RequestMethod &method, const std::string &route, RouteHandler handler) {
  Router& router = getInstance();
  router.routeMapping[route][method] = std::move(handler);
}

bool Quark::Router::isValidRoute(const std::string &path) {
  return routeMapping.find(path) != routeMapping.end() || FileHelper::isValidFilePath(staticServePath + path);
}

// THIS SHOULD ONLY BE CALLED IF WE'RE 100% SURE THE ROUTE IS VALID
Quark::HttpResponse Quark::Router::handleRequest(HttpRequest& request) {
  RequestMethod requestMethod = request.getRequestMethod();
  std::string requestPath = request.path;

  if (
    routeMapping.find(requestPath) != routeMapping.end() &&
    routeMapping[requestPath].find(requestMethod) != routeMapping[requestPath].end()
  ) {
    return routeMapping[requestPath][requestMethod](request);   
  } else if (FileHelper::isValidFilePath(staticServePath + requestPath)) {
    return HttpResponse::sendFile(staticServePath + requestPath);
  }

  throw std::runtime_error("Unroutable request");
}

Quark::HttpResponse Quark::Router::handleOptionsRequest(std::string &path) {
  if (!isValidRoute(path)) return HttpResponse::notFound();

  HttpResponse response = HttpResponse::ok();

  std::ostrstream oss;

  int i = 0;
  bool canGET = false;
  for (auto it : routeMapping[path]) {
    if (i > 0) oss << ", ";
    if (it.first == RequestMethod::GET) canGET = true;

    oss << requestMethodToString(it.first); 
    i++;
  }

  if (i > 0) oss << ", ";

  oss << "OPTIONS";

  bool isValidFilePath = FileHelper::isValidFilePath(staticServePath + path);

  // We may not have an explicit path defined for GET if this is a file route, so we 
  // need to set that allow-method manually
  if (!canGET && isValidFilePath) oss << ", GET";

  if (routeMapping[path].find(RequestMethod::GET) != routeMapping[path].end() || isValidFilePath) {
    oss << ", HEAD";
  }

  return response.addHeader("Allow", oss.str());
} 

Quark::HttpResponse Quark::Router::handleHeadRequest(std::string &path, HttpRequest &request) {
  if (!isValidRoute(path)) return HttpResponse::notFound();

  return handleRequest(request).setBody("", false);
}

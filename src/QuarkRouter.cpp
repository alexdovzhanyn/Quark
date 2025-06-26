#include "QuarkRouter.hpp"
#include "HttpRequestParser.hpp"
#include "RequestMethod.hpp"
#include "util/FileHelper.hpp"
#include "RouteNode.hpp"
#include <memory>
#include <stdexcept>
#include <strstream>
#include <vector>

void Quark::Router::GET(std::string route, RouteHandler handler) {
  addRoute(RequestMethod::GET, route, handler);
}

void Quark::Router::PATCH(std::string route, RouteHandler handler) {
  addRoute(RequestMethod::PATCH, route, handler);
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
  std::shared_ptr<RouteNode> routeNode = parsePathParams(route);

  if (routeNode == nullptr) {
    router.simpleRoutes[route][method] = std::move(handler);
    return;
  } 

  std::optional<RouteTable*> table = router.findRouteTableForRoute(route);

  if (table.has_value()) {
    (*table.value())[method] = std::move(handler);
    return;
  }

  RouteTable newTable;
  newTable[method] = std::move(handler);
  router.complexRoutes[routeNode] = std::move(newTable);
}

std::shared_ptr<Quark::RouteNode> Quark::Router::parsePathParams(const std::string &route) {
  if (route.find(':') == std::string::npos) return nullptr;

  std::shared_ptr<RouteNode> root = std::make_shared<RouteNode>("/");
  std::shared_ptr<RouteNode> currNode = root;

  std::string currChunk;
  for (int i = 1; i < route.length(); i++) {
    if (route[i] != '/') currChunk += route[i];

    if (route[i] == '/' || i == route.length() - 1) {
      currNode->next = std::make_shared<RouteNode>(currChunk, currChunk[0] == ':');
      currNode = currNode->next;
      currChunk = "";
    }
  }

  return root->next;
}

std::optional<Quark::Router::RouteTable*> Quark::Router::findRouteTableForRoute(const std::string &path) {
  auto it = simpleRoutes.find(path);
  if (it != simpleRoutes.end()) return &it->second;

  std::shared_ptr<RouteNode> route = findMatchingRoute(path);
  if (route) return &complexRoutes[route];

  return std::nullopt;
}

std::shared_ptr<Quark::RouteNode> Quark::Router::findMatchingRoute(const std::string &path) {
  for (const auto &[node, table] : complexRoutes) {
    if (node->matches(path)) return node;
  }

  return nullptr;
}

bool Quark::Router::isValidRoute(const std::string &path) {
  return findRouteTableForRoute(path).has_value() || FileHelper::isValidFilePath(staticServePath + path);
}

// THIS SHOULD ONLY BE CALLED IF WE'RE 100% SURE THE ROUTE IS VALID
Quark::HttpResponse Quark::Router::handleRequest(HttpRequest& request) {
  RequestMethod requestMethod = request.getRequestMethod();
  std::string requestPath = request.path;

  std::optional<RouteTable*> routeTable = findRouteTableForRoute(requestPath);

  if (
    routeTable.has_value() &&
    (*routeTable.value()).find(requestMethod) != (*routeTable.value()).end()
  ) {
    HttpRequestParser::parsePathParams(request, findMatchingRoute(requestPath));

    return (*routeTable.value())[requestMethod](request);   
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

  std::optional<RouteTable*> routeTable = findRouteTableForRoute(path);
  if (routeTable.has_value()) {
    for (auto it : (*routeTable.value())) {
      if (i > 0) oss << ", ";
      if (it.first == RequestMethod::GET) canGET = true;

      oss << requestMethodToString(it.first); 
      i++;
    }  
  }

  if (i > 0) oss << ", ";

  oss << "OPTIONS";

  bool isValidFilePath = FileHelper::isValidFilePath(staticServePath + path);

  // We may not have an explicit path defined for GET if this is a file route, so we 
  // need to set that allow-method manually
  if (!canGET && isValidFilePath) oss << ", GET";

  if (canGET || isValidFilePath) oss << ", HEAD";

  return response.addHeader("Allow", oss.str());
} 

Quark::HttpResponse Quark::Router::handleHeadRequest(std::string &path, HttpRequest &request) {
  if (!isValidRoute(path)) return HttpResponse::notFound();

  return handleRequest(request).setBody("", false);
}

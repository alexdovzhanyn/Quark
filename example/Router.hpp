#pragma once
#include "QuarkRouter.hpp"
#include "controllers/ApplicationController.hpp"
#include "controllers/UserController.hpp"

class Router {
public:
  static void registerRoutes() {
    Quark::Router::GET("/", ApplicationController::getRoot);
    Quark::Router::GET("/hello", ApplicationController::helloWorld);
    Quark::Router::PATCH("/user/id", UserController::updateUser);
    Quark::Router::POST("/user/create", UserController::createUser);
  }
};

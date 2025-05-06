#pragma once
#include "QuarkRouter.hpp"
#include "controllers/ApplicationController.hpp"
#include "controllers/UserController.hpp"

class Router {
public:
  static void registerRoutes() {
    Quark::Router &router = Quark::Router::getInstance();

    router.get("/", ApplicationController::getRoot);
    router.get("/hello", ApplicationController::helloWorld);

    router.post("/user/create", UserController::createUser);
  }
};

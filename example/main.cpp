#include "QuarkServer.hpp"
#include "Router.hpp"

int main() {
  Quark::Server server = Quark::Server("9669");

  Router::registerRoutes();

  server.run();
}

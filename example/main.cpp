#include "QuarkServer.hpp"
#include "Router.hpp"

int main() {
  Quark::Server server = Quark::Server("9669");

  server
    .registerResponseMiddleware([](Quark::HttpResponse& response) {
      response.addHeader("Access-Control-Allow-Origin", "*");
    });

  Router::registerRoutes();

  server.run();
}

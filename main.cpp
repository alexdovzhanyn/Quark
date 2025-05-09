#include "src/QuarkServer.hpp"
#include "src/QuarkRouter.hpp"

int main() {
  Quark::Server server = Quark::Server("9669");

  Quark::Router &router = Quark::Router::getInstance();

  router.POST("/meow/cow", [](Quark::HttpRequest &request) {
    return Quark::HttpResponse::ok(R"({
      "name": "Douglas",
      "age": 23
    })")
    .addHeader("Content-Type", "text/json");
  });

  server.run();
}


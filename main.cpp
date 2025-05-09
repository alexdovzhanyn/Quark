#include "src/QuarkServer.hpp"
#include "src/QuarkRouter.hpp"

int main() {
  Quark::Server server = Quark::Server("9669");

  Quark::Router &router = Quark::Router::getInstance();

  router.POST("/meow/cow", [](Quark::HttpRequest &request, Quark::HttpResponse &response) {
    std::cout << "GOT HIT!" << std::endl;
    response.setStatus(200);
    response.addHeader("Content-Type", "text/json");
    response.setBody(R"({
      "name": "Douglas",
      "age": 23
    })");
  });

  server.run();
}


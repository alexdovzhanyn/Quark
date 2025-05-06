#include "src/QuarkServer.hpp"
#include "src/QuarkRouter.hpp"

int main() {
  QuarkServer server = QuarkServer("9669");

  QuarkRouter &router = QuarkRouter::getInstance();

  router.post("/meow/cow", [](HttpRequest &request, HttpResponse &response) {
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


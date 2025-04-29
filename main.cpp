#include <iostream>
#include "src/QuarkServer.hpp"

int main() {
  QuarkServer server = QuarkServer("9669");

  server.run();
}


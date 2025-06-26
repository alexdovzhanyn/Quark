#pragma once
#include <memory>
#include <string>

namespace Quark {
  class RouteNode {
  public:
    RouteNode(std::string pathChunk, bool isWild = false) : val(pathChunk), isWildcard(isWild) {}

    std::shared_ptr<RouteNode> next;
    std::string val;
    bool isWildcard;

    bool matches(const std::string &route, int i = 1);
  };
}

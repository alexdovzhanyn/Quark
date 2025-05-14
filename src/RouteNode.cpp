#include "RouteNode.hpp"

bool Quark::RouteNode::matches(const std::string &route, int i) {
  std::string curr;

  while (i < route.length() && route[i] != '/') {
    curr += route[i];
    i++;
  }

  bool isMatch = isWildcard || curr == val;

  if (!isMatch) return false;
  if (next == nullptr) return i >= route.length();
  if (i >= route.length()) return false; 

  return next->matches(route, i + 1);
}

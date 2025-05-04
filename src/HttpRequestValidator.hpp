#include "RequestMethod.hpp"
#include <string>
#include <unordered_map>

class HttpRequestValidator {
public:
  static void validateMethod(std::string method);
  static void validateProtocolVersion(std::string version);

private: 
  static const std::unordered_map<std::string, RequestMethod> methodMapping; 
  static RequestMethod methodStringToEnum(std::string method);
};

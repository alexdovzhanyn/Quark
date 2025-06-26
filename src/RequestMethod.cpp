#include "RequestMethod.hpp"

std::string Quark::requestMethodToString(Quark::RequestMethod method) {
  switch (method) {
    case RequestMethod::GET: return "GET";
    case RequestMethod::PATCH: return "PATCH";
    case RequestMethod::POST: return "POST";
    case RequestMethod::PUT: return "PUT";
    case RequestMethod::DELETE: return "DELETE";
    case RequestMethod::OPTIONS: return "OPTIONS";
    default: return "HEAD";
  }
}

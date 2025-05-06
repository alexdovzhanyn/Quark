#pragma once
#include "RequestMethod.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>

namespace Quark {
  class HttpRequest {
  public:
    std::string ip;
    std::string path;
    std::string method;
    std::string queryString;
    std::string protocolVersion;
    std::unordered_map<std::string, std::string> queryParams;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    
    HttpRequest(std::string ipAddr) : ip(ipAddr) {}

    RequestMethod getRequestMethod() {
      if (method == "GET") return RequestMethod::GET;
      if (method == "POST") return RequestMethod::POST;
      if (method == "DELETE") return RequestMethod::DELETE;
      if (method == "PUT") return RequestMethod::PUT;
      if (method == "HEAD") return RequestMethod::HEAD;

      return RequestMethod::OPTIONS;
    }

    std::string asJson() {
      std::ostringstream oss;
      oss << "{" << std::endl;
      oss << "  method: '" << method << "'," << std::endl;
      oss << "  path: '" << path << "'," << std::endl;
      oss << "  queryString: '" << queryString << "'," << std::endl;
      oss << "  queryParams: {" << std::endl;
      
      int i = 0;
      for (auto it = queryParams.begin(); it != queryParams.end(); it++) {
        if (i > 0) oss << "," << std::endl;
        oss << "    '" << it->first << "': '" << it->second << "'"; 
        i++;
      }

      oss << std::endl << "  }," << std::endl;
      oss << "  headers: {" << std::endl;

      i = 0;
      for (auto it = headers.begin(); it != headers.end(); it++) {
        if (i > 0) oss << "," << std::endl;
        oss << "    '" << it->first << "': '" << it->second << "'"; 
        i++;
      }

      oss << std::endl << "  }" << std::endl;
      oss << "}" << std::endl;

      return oss.str();
    }
  };
}

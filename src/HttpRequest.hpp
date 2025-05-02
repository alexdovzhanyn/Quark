#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

class HttpRequest {
public:
  std::string path;
  std::string method;
  std::unordered_map<std::string, std::string> queryParams;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
  
  bool parsedStartLine = false;
  bool parsedHeaders = false;

  HttpRequest() = default;

  void addHeader(std::string headerName, std::string headerValue) {
    headers.insert({ headerName, headerValue });
  }


  void parseStartLine(std::string &request, int &i) {
    method = accumulateUntil(' ', request, i);
    path = accumulateUntil({'?', ' '}, request, i);

    if (request[i - 1] != ' ') parseQueryParams(request, i);

    // Ignore the protocol, just move on until we hit the next line
    accumulateUntil('\n', request, i);
    parsedStartLine = true;
  }

  void parseHeaders(std::string &request, int &i) {
    while (i < request.length() && request[i] != '\r') {
      std::string headerName = accumulateUntil(':', request, i);
      i++; // Skip the space between kv pairs
      std::string headerValue = accumulateUntil('\r', request, i);
      i++; // Skip the \n between headers

      headers.insert({ headerName, headerValue });
    }

    i += 2; // Skip the \r\n at the end of the header section

    parsedHeaders = true;
  }

  void printAsJson() {
    std::cout << "{" << std::endl;
    std::cout << "  method: '" << method << "'," << std::endl;
    std::cout << "  path: '" << path << "'," << std::endl;
    std::cout << "  queryParams: {" << std::endl;
    
    int i = 0;
    for (auto it = queryParams.begin(); it != queryParams.end(); it++) {
      if (i > 0) std::cout << "," << std::endl;
      std::cout << "    '" << it->first << "': '" << it->second << "'"; 
      i++;
    }

    std::cout << std::endl << "  }," << std::endl;
    std::cout << "  headers: {" << std::endl;

    i = 0;
    for (auto it = headers.begin(); it != headers.end(); it++) {
      if (i > 0) std::cout << "," << std::endl;
      std::cout << "    '" << it->first << "': '" << it->second << "'"; 
      i++;
    }

    std::cout << std::endl << "  }" << std::endl;
    std::cout << "}" << std::endl;
  }

private:

  void parseQueryParams(std::string &request, int &i) {
    while (i < request.length() && request[i] != ' ') {
      std::string paramName = accumulateUntil('=', request, i, {'?'});
      std::string paramValue = accumulateUntil({'&', ' '}, request, i);

      queryParams.insert({ paramName, paramValue });

      // In case we skipped past the end of the queryString by accident
      if (request[i - 1] == ' ') break;
    }
  }

  std::string accumulateUntil(char endChar, std::string &chunk, int &i, std::unordered_set<char> skipChars = {}) {
    return accumulateUntil(std::unordered_set<char>{ endChar }, chunk, i, skipChars);
  }

  std::string accumulateUntil(std::unordered_set<char> delimiters, std::string &chunk, int &i, std::unordered_set<char> skipChars = {}) {
    std::string acc;
    while (i < chunk.length() && delimiters.find(chunk[i]) == delimiters.end()) {
      if (skipChars.find(chunk[i]) != skipChars.end()) {
        i++;
        continue;
      }

      acc += chunk[i];
      i++;
    }  

    // This means we hit a delimiter, not that we're at the end of the string
    // so we can skip the delimiter as well.
    if (i < chunk.length()) i++;

    return acc;
  }
};

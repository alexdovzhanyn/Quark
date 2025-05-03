#include "HttpRequestParser.hpp"
#include <string>
#include <sys/socket.h>
#include <unordered_set>
#include <vector>

bool HttpRequestParser::parseIncomingRequest() {
  std::vector<char> buffer(RECV_BUFFER_SIZE);
  ssize_t packetSize;

  // Some clients seem to make extra empty dummy requests for some reason, which just immediately
  // open and close the connection. We don't want to bother logging those for now
  bool isRealRequest = false;
  while ((packetSize = recv(socketDescriptor, buffer.data(), buffer.size(), 0)) > 0) {
    isRealRequest = true;

    rawRequest.insert(rawRequest.end(), buffer.begin(), buffer.begin() + packetSize);
    for (int i = 0; i < packetSize; i++) {
      rawRequest.push_back(buffer[i]);
    }

    if (parserState == ParserState::PARSE_START_LINE) {
      // We haven't even received all the start-line data yet. Wait until we do.
      if (!HttpRequestParser::queueContainsSequential("\r\n", rawRequest)) continue;

      parseStartLine();
    } 

    if (parserState == ParserState::PARSE_HEADERS) {
      // We haven't finished receiving the headers yet. Wait until we have.
      if (!HttpRequestParser::queueContainsSequential("\r\n\r\n", rawRequest)) continue;

      parseHeaders();
    }

    if (request.headers.find("Content-Length") == request.headers.end()) break;

    // TODO: parse the body or something here
    if (std::stoul(request.headers.at("Content-Length")) == rawRequest.size()) break;
  }

  return isRealRequest;
}

void HttpRequestParser::parseStartLine() {
  request.method = accumulateUntil(' ');
  request.path = accumulateUntil({'?', ' '});

  // If it weren't for the need to iterate over the rawRequest here, we could've used a queue
  // instead of a deque. It is what it is
  if (prevChar != ' ') {
    request.queryString += '?';

    for (char curr : rawRequest) {
      if (curr == ' ') break;

      request.queryString += curr;
    }

    parseQueryParams();
  }

  request.protocolVersion = accumulateUntil('\r');
  rawRequest.pop_front();

  advanceParserState();
}

void HttpRequestParser::parseHeaders() {
  while (!rawRequest.empty() && rawRequest.front() != '\r') {
    std::string headerName = accumulateUntil(':');
    rawRequest.pop_front(); // Skip the space between kv pairs

    std::string headerValue = accumulateUntil('\r');
    rawRequest.pop_front(); // Skip the \n between headers

    request.headers.insert({ headerName, headerValue });
  }
  
  // Skip the \r\n
  rawRequest.pop_front();
  rawRequest.pop_front();

  advanceParserState();
}

void HttpRequestParser::parseQueryParams() {
  while (!rawRequest.empty() && rawRequest.front() != ' ') {
    std::string paramName = accumulateUntil('=', {'?'});
    std::string paramValue = accumulateUntil({'&', ' '});

    request.queryParams.insert({ paramName, paramValue });

    // In case we skipped past the end of the queryString by accident
    if (prevChar == ' ') break;
  }
}

std::string HttpRequestParser::accumulateUntil(char delimiter, const std::unordered_set<char> &skipChars) {
  return accumulateUntil(std::unordered_set<char>{ delimiter }, skipChars);
}

std::string HttpRequestParser::accumulateUntil(const std::unordered_set<char> &delimiters, const std::unordered_set<char> &skipChars) {
  std::string acc;

  while (!rawRequest.empty() && delimiters.find(rawRequest.front()) == delimiters.end()) {
    char currentChar = rawRequest.front();
    rawRequest.pop_front();

    if (skipChars.find(currentChar) != skipChars.end()) continue;

    acc += currentChar;

    prevChar = currentChar;
  }

  // This means we hit a delimiter, not that we're at the end of the string
  // so we can skip the delimiter as well.
  if (!rawRequest.empty()) {
    prevChar = rawRequest.front();
    rawRequest.pop_front();
  }

  return acc;
}

bool HttpRequestParser::queueContainsSequential(const std::string &chars, const std::deque<char> &q) {
  if (chars.empty() || q.empty() || chars.size() > q.size()) return false;

  for (auto it = q.begin(); it != q.end(); ++it) {
    if (*it != chars[0]) continue;

    int i = 0;
    while (i < chars.size() && it + i != q.end() && *(it + i) == chars[i]) ++i;

    if (i == chars.length()) return true;
  }

  return false;
}

void HttpRequestParser::advanceParserState() {
  if (parserState == ParserState::PARSE_START_LINE) parserState = ParserState::PARSE_HEADERS;
  else if (parserState == ParserState::PARSE_HEADERS) parserState = ParserState::PARSE_BODY;
  else parserState = ParserState::FINISHED;
}

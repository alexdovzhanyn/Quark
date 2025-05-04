#pragma once
#include "HttpRequest.hpp"
#include <deque>
#include <unordered_set>

enum ParserState {
  PARSE_REQUEST_LINE,
  PARSE_HEADERS,
  PARSE_BODY,
  FINISHED
};

class HttpRequestParser {
public:
  HttpRequestParser(int &sock, HttpRequest &req) : request(req), socketDescriptor(sock) {} 

  bool parseIncomingRequest();
private:
  int RECV_BUFFER_SIZE = 8192;
  char prevChar;
  std::deque<char> rawRequest;
  ParserState parserState = ParserState::PARSE_REQUEST_LINE;
  HttpRequest &request;
  int &socketDescriptor;

  void parseRequestLine();

  void parseHeaders();

  void parseQueryParams();

  std::string accumulateUntil(char delimiter, const std::unordered_set<char> &skipChars = {});
  std::string accumulateUntil(const std::unordered_set<char> &delimiters, const std::unordered_set<char> &skipChars = {});

  static bool queueContainsSequential(const std::string &chars, const std::deque<char> &q);

  void advanceParserState();
};

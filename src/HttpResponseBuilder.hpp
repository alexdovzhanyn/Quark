#include <string>

class HttpResponseBuilder {
public:
  HttpResponseBuilder() = default;
  
  void setStatus(int status, std::string message = "");
  void setBody(std::string bod);

  std::string str();
private:
  std::string protocolVersion = "HTTP/1.1";
  int statusCode;
  std::string statusMessage;
  std::string responseBody;
};


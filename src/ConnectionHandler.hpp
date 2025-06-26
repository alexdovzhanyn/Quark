#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <functional>

using RequestMiddlewareHandler = std::function<void(Quark::HttpRequest&)>;
using ResponseMiddlewareHandler = std::function<void(Quark::HttpResponse&)>;

namespace Quark {
  class ConnectionHandler {
  public:
    ConnectionHandler(
      int sock,
      sockaddr_storage addr,
      const std::vector<RequestMiddlewareHandler> &reqMiddlewares,
      const std::vector<ResponseMiddlewareHandler> &resMiddlewares
    ) : socketDescriptor(sock), connectionAddress(addr), requestMiddlewares(reqMiddlewares), responseMiddlewares(resMiddlewares) {}

    void handleHttpRequest();

  private:
    int socketDescriptor;
    sockaddr_storage connectionAddress;
    const std::vector<RequestMiddlewareHandler> requestMiddlewares;
    const std::vector<ResponseMiddlewareHandler> responseMiddlewares;
    const int MAX_REQUEST_TIMEOUT = 30; // Seconds
    const int MAX_KEEPALIVE_REQUESTS = 100;

    int requestsProcessed = 0;

    void applyRequestMiddleware(HttpRequest &request);
    void applyResponseMiddleware(HttpResponse &response);

    int awaitActivity();
    void closeConnection();
    
    void logRequest(
      const std::chrono::time_point<std::chrono::steady_clock> &requestEnd,
      const HttpRequest &request,
      const int &statusCode
    );

    std::string getIpFromSocket(); 
  };
}

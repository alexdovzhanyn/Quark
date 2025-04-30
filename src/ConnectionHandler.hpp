#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

class ConnectionHandler {
public:
  ConnectionHandler(int sock, sockaddr_storage addr) : socketDescriptor(sock), connectionAddress(addr) {}

  void attendConnection() {
    char buffer[RECV_BUFFER_SIZE];
  
    ssize_t bytesReceived;
    while ((bytesReceived = recv(socketDescriptor, &buffer, RECV_BUFFER_SIZE, 0)) > 0) {
      std::cout << "Got some data for socket " << std::to_string(socketDescriptor) << std::endl;
      std::cout.write(buffer, bytesReceived); 

      send(socketDescriptor, "OK", 2, 0);
    }

    close(socketDescriptor);
  }
private:
  int socketDescriptor;
  sockaddr_storage connectionAddress;
  int RECV_BUFFER_SIZE = 8192;
};

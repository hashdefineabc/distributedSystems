#ifndef SOCKET_H
#define SOCKET_H

#include <stdexcept>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class SocketException : public std::runtime_error {
public:
  SocketException(const std::string &message) : std::runtime_error(message) {}
};

class Socket {
private:
  int fd;

public:
  Socket(int fd);
  Socket(Socket &&other) : fd(other.fd) { other.fd = -1; }
  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;
  ~Socket();
  int getFd() { return fd; }
  Socket accept();
  void sendData(const std::string &message);
  std::string receiveData();

  static Socket connect(const std::string &host, int port);
  static Socket listen(int port);
};

#endif // !SOCKET_H

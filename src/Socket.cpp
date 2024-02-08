#include "Socket.h"
#include <iostream>

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// For socket communication, sending and receiving data over TCP sockets.

Socket::Socket(int fd) : fd(fd) {}

Socket::~Socket() {
  if (fd != -1) {
    ::close(fd); 
  }
}

Socket Socket::connect(const std::string &host, int port) {
  struct addrinfo hints, *servinfo, *p;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int rv;
  if ((rv = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints,
                        &servinfo)) != 0) {
    throw SocketException(std::string("getaddrinfo: ") +
                          std::string(gai_strerror(rv)));
  }

  int sockfd;
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      continue;
    }
    if (::connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      ::close(sockfd);
      std::cerr << "Connect: " << strerror(errno) << std::endl;
      continue;
    }
    break;
  }

  if (p == NULL) {
    throw SocketException("Binding failed");
  }

  freeaddrinfo(servinfo);

  return Socket(sockfd);
}

Socket Socket::listen(int port) {
  struct addrinfo hints, *servinfo, *p;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int rv;
  if ((rv = getaddrinfo(NULL, std::to_string(port).c_str(), &hints,
                        &servinfo)) != 0) {
    throw SocketException(std::string("getaddrinfo: ") +
                          std::string(gai_strerror(rv)));
  }

  int sockfd;
  int yes = 1;
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      std::cerr << "Socket: " << strerror(errno) << std::endl;
      continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      throw SocketException(std::string("setsockopt: ") +
                            std::string(strerror(errno)));
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      ::close(sockfd);
      continue;
    }
    break;
  }

  if (p == NULL) {
    throw SocketException("Binding failed");
  }

  freeaddrinfo(servinfo);

  if (::listen(sockfd, 10) == -1) {
    throw SocketException("Listen");
  }

  return Socket(sockfd);
}

Socket Socket::accept() {
  int new_fd;
  if ((new_fd = ::accept(fd, NULL, NULL)) == -1) {
    throw SocketException(std::string("Accept: ") + std::string(strerror(errno)));
  }
  return Socket(new_fd);
}

void Socket::sendData(const std::string &message) {
  // std::cerr << "sending: " << message.size() << std::endl;
  uint32_t len = htonl(message.size());
  ssize_t sent = ::send(fd, &len, sizeof len, 0);
  if (sent == -1) {
    throw SocketException(std::string("Send len: ") + std::string(strerror(errno)));
  }
  if (::send(fd, message.c_str(), message.size(), 0) == -1) {
    throw SocketException(std::string("Send msg: ") + std::string(strerror(errno)));
  }
}

std::string Socket::receiveData() {
  uint32_t len;
  ssize_t recvd = ::recv(fd, &len, sizeof len, 0);
  if (recvd == -1) {
    throw SocketException(std::string("Received len: ") + std::string(strerror(errno)));
  }
  if (recvd == 0) {
    return "";
  }
  len = ntohl(len);
  std::string message(len, 0);

  recvd = ::recv(fd, &message[0], len, 0);
  if (recvd == -1) {
    throw SocketException(std::string("Received msg: ") + std::string(strerror(errno)));
  }
  if (recvd == 0) {
    return "";
  }
  return message;
}

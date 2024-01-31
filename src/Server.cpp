#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Server {
private:
    int sockfd, newSock;
    struct sockaddr_in serverAddr, newAddr;
    socklen_t addrSize;

public:
    Server(int serverPort) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket" << std::endl;
            exit(1);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        serverAddr.sin_addr.s_addr = INADDR_ANY;
    }

    void bindAndListen() {
        int opt = 1;

        // Set the SO_REUSEADDR option
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt )) < 0) {
            std::cerr << "Error setting socket option" << std::endl;
            exit(1);
        }

        if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error binding" << std::endl;
            exit(1);
        }

        listen(sockfd, 1);
        addrSize = sizeof(newAddr);
        newSock = accept(sockfd, (struct sockaddr*)&newAddr, &addrSize);
        if (newSock < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            exit(1);
        }
        std::cout << "Client connected" << std::endl;
    }

    std::string receiveData() {
        char buffer[1024];
        recv(newSock, buffer, sizeof(buffer), 0);
        return std::string(buffer);
    }

    int getNewSocket() const {
        return newSock;
    }

    ~Server() {
        close(newSock);
        close(sockfd);
    }
};

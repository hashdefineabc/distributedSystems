#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Client {
private:
    int sockfd;
    struct sockaddr_in serverAddr;

public:
    Client(const std::string& serverIP, int serverPort) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket" << std::endl;
            exit(1);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);
    }

    void connectToServer() {
        if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error connecting to the server" << std::endl;
            exit(1);
        }
        std::cout << "Connected to the server" << std::endl;
    }

    void sendData(const std::string& data) {
        send(sockfd, data.c_str(), data.size(), 0);
    }

    int getSocket() const {
        return sockfd;
    }

    void closeConnection() {
        close(sockfd);
        std::cout << "Connection closed " << std::endl;
    }

    ~Client() {
        close(sockfd);
    }
};

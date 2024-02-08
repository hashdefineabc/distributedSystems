#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Client
{
private:
    int sockfd;              // Socket file descriptor
    struct sockaddr_in addr; // Server address structure
    std::mutex mtx; // Mutex for synchronization

public:
    // Constructor: Initializes a client and connects to the server.
    Client(const std::string &serverIP, int serverPort)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
        if (sockfd < 0)
        {
            perror("ERROR: failed to create socket");
            exit(1);
        }

        memset(&addr, 0, sizeof(addr));                     // Initialize server address structure
        addr.sin_family = AF_INET;                          // Address family (IPv4)
        addr.sin_port = htons(serverPort);                  // Port in network byte order
        addr.sin_addr.s_addr = inet_addr(serverIP.c_str()); // Server IP address

        // Connect to the server
        if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("ERROR: failed to connect");
            exit(1);
        }
    }

    // Send data to the server
    void sendData(const std::string &data)
    {
        std::lock_guard<std::mutex> lock(mtx); // Lock the mutex
        int length = send(sockfd, data.c_str(), data.size(), 0); // Send data
        if (length < 0)
        {
            perror("ERROR: failed to send");
            exit(1);
        }
    }

    // Receive data from the server
    std::string receiveData()
    {
        std::lock_guard<std::mutex> lock(mtx); // Lock mutex
        char buffer[1024];                                    // Adjust buffer size as needed
        int length = recv(sockfd, buffer, sizeof(buffer), 0); // Receive data
        if (length < 0)
        {
            perror("ERROR from Client: failed to receive");
            exit(1);
        }
        return std::string(buffer, length); // Create a string from received data
    }

    // void closeConnection() {
    //     std::lock_guard<std::mutex> lock(mtx); // Lock mutex
    //     close(sockfd);
    //     std::cout << "Connection closed " << std::endl;
    // }

    // Destructor: Close the socket when the object is destroyed
    ~Client()
    {
        std::lock_guard<std::mutex> lock(mtx); // Lock mutex
        close(sockfd);
    }
};

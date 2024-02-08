#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BACKLOG 2000

class Server
{
private:
    int sockfd, newfd;                // Socket file descriptors
    struct sockaddr_in my_addr, addr; // Server and client address structures
    unsigned int addr_size = sizeof(addr);
    int port; // Port number

public:
    // Constructor: Initializes a server and binds to a specified port.
    Server(int serverPort) : port(serverPort)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
        if (sockfd < 0)
        {
            perror("ERROR: failed to create socket");
            exit(1);
        }

        memset(&my_addr, '\0', sizeof(my_addr)); // Initialize server address structure
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(port);
        my_addr.sin_addr.s_addr = INADDR_ANY;

        // Bind the socket to the server's address
        if ((bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr))) < 0)
        {
            perror("ERROR: failed to bind");
            exit(1);
        }

        // Listen for incoming connections
        listen(sockfd, BACKLOG);

        // Accept an incoming connection
        newfd = accept(sockfd, (struct sockaddr *)&addr, &addr_size);
        if (newfd < 0)
        {
            perror("ERROR: failed to accept");
            exit(1);
        }
    }

    // Receive data from the connected client
    std::string receiveData()
    {
        char buffer[1024]; // Adjust buffer size as needed
        int length = recv(newfd, buffer, sizeof(buffer), 0);
        if (length < 0)
        {
            perror("ERROR from Server: failed to receive");
            exit(1);
        }
        return std::string(buffer, length); // Create a string from received data
    }

    // Send data to the connected client
    void sendData(const std::string &data)
    {
        int length = send(newfd, data.c_str(), data.size(), 0);
        if (length < 0)
        {
            perror("ERROR: failed to send");
            exit(1);
        }
    }

    // Destructor: Close the client and server sockets when the object is destroyed
    ~Server()
    {
        close(newfd);
        close(sockfd);
    }
};

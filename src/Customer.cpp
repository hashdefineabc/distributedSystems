#include <iostream>
#include <string>
#include "Client.cpp"
#include "Order.cpp"
#include "LaptopInfo.cpp"

class Customer {
    public:
        Customer(int id, const std::string& ip, int port, int numOrders, int laptopType ) : id(id), ip(ip), port(port), numOrders(numOrders), laptopType(laptopType) {}
        
        void run() {
            Client client(ip, port);
            client.connectToServer();

            for (int i = 0; i < numOrders; i++) {
                Order order(id, i+1, laptopType);
                
                std::string orderData = order.serialize();
                client.sendData(orderData);

                char buffer[1024];
                int bytesRead = recv(client.getSocket(), buffer, sizeof(buffer), 0);
                if(bytesRead < 0) {
                    std::cerr << "Error receiving data from the server" << std::endl;
                    exit(1);
                }

                std::string receivedData(buffer, bytesRead);
                LaptopInfo laptopInfo;
                laptopInfo.deserialize(receivedData);

                std::cout << "Customer " << id << " - LaptopInfo received from Server:\n" << laptopInfo.toString() << std::endl;

            }

            client.closeConnection();
        }

    private:
        int id;
        std::string ip;
        int port;
        int numOrders;
        int laptopType;
};
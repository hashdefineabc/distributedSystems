#include "Socket.h"
#include "Order.h"
#include "LaptopInfo.h"

class ServerStub {
private:
    Socket socket;

public:
    ServerStub(Socket &&socket) : socket(std::move(socket)) {}

    Order ReceiveOrder() {
        Order order;
        try {
            // Receive the order in byte stream format
            std::string orderDataReceived = socket.receiveData();
            if(!orderDataReceived.empty()) {
                order = Order::UnmarshalOrder(orderDataReceived);
            }
            return order;
        } catch (const SocketException &e) {
            std::cerr << "Error receiving order: " << e.what() << std::endl;
            return order;
        }
    }

    void ShipLaptop(const LaptopInfo& laptopInfo) {
        try {
            // Marshal laptop information into a byte stream and send it
            std::string laptopData = laptopInfo.MarshalLaptopInfo();
            socket.sendData(laptopData);
        } catch (const SocketException &e) {
            std::cerr << "Error shipping laptop: " << e.what() << std::endl;
            return;
        }
    }
};

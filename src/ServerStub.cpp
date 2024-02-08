#include "Socket.h"
#include "Order.h"
#include "LaptopInfo.h"

class ServerStub {
private:
    Socket socket;

public:
    ServerStub(Socket &&socket) : socket(std::move(socket)) {}

    Order ReceiveOrder() {
        // Receive the order in byte stream format
        std::string orderDataReceived = socket.receiveData();
        Order order = Order::UnmarshalOrder(orderDataReceived);
        return order;
    }

    void ShipLaptop(const LaptopInfo& laptopInfo) {
        // Marshal laptop information into a byte stream and send it
        std::string laptopData = laptopInfo.MarshalLaptopInfo();
        socket.sendData(laptopData);
    }
};

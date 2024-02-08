#include "Socket.h"
#include "LaptopInfo.h"
#include "Order.h"

class ClientStub {
private:
    Socket socket;

public:
    ClientStub(const std::string& serverIP, int serverPort)
        : socket(Socket::connect(serverIP, serverPort)) {}

    LaptopInfo Order(const Order& order) {

        socket.sendData(order.MarshalOrder());
        std::string laptopInfoReceived = socket.receiveData();

        LaptopInfo laptopInfo = LaptopInfo::UnmarshalLaptopInfo(laptopInfoReceived);
        
        return laptopInfo;
    }

};

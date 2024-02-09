#include "Socket.h"
#include "LaptopInfo.h"
#include "Order.h"

class ClientStub {
private:
    Socket socket;

public:
    ClientStub(const std::string& serverIP, int serverPort)
        : socket(Socket::connect(serverIP, serverPort)) {}

    bool OrderLaptop(const Order& order, LaptopInfo& laptopInfo) {
       // LaptopInfo laptopInfo;

        try {
            // Send order to server
            socket.sendData(order.MarshalOrder());

            //std::cout<<"send order to server: CustId: " << order.customer_id << " orderNumber: " << order.order_number << " laptop_type " << order.laptop_type << std::endl;
            
            // Receive laptop information from server
            std::string laptopInfoReceived = socket.receiveData();

            if(laptopInfoReceived.empty()) {
                //std::cerr<<"Error receiving laptop information from server, customer order failed "<<std::endl;
                return false;
            }

            //std::cout << "laptop info received: " << laptopInfoReceived << std::endl;

            // Unmarshal received laptop information
            laptopInfo = LaptopInfo::UnmarshalLaptopInfo(laptopInfoReceived);

            return true;
        } catch (const SocketException& e) {
            std::cerr << "Error ordering laptop: " << e.what() << std::endl;
            return false;
        }
    }

};

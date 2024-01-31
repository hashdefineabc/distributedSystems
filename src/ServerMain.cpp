#include <iostream>
#include "Server.cpp"
#include "Order.cpp"
#include "LaptopInfo.cpp"

Server init() {
	Server server(8080);
	server.bindAndListen();

	std::string receivedData = server.receiveData();
	std::cout << "Received data from client: " << receivedData << std::endl;
	return server;
}

Order receiveOrder(Server& server) {
    std::string orderData = server.receiveData();
    Order order;
    order.deserialize(orderData); // Assuming you have a deserialize method in the Order class
    return order;
}

void shipLaptop(Server& server, const LaptopInfo& laptopInfo) {
    std::string laptopData = laptopInfo.serialize(); // Assuming you have a serialize method in LaptopInfo
    send(server.getNewSocket(), laptopData.c_str(), laptopData.size(), 0);
}

int main(int argc, char *argv[]) {
	Server server = init();
	Order receivedOrder = receiveOrder(server);
    std::cout << "\nReceived Order from client:\n" << receivedOrder.toString() << std::endl;

    LaptopInfo assembledLaptop(receivedOrder.getCustomerId(), receivedOrder.getOrderNumber(), receivedOrder.getLaptopType(), 789, 101);
    
    // Ship the laptop information to the customer
    shipLaptop(server, assembledLaptop);
	return 0;
}

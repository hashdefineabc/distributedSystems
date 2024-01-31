#include <iostream> 
#include <thread>
#include "Client.cpp"
#include "LaptopInfo.cpp"
#include "Order.cpp"

Client init(std::string ip , int port){
	Client client(ip, port);
    client.connectToServer();
    
    std::string data = "Hello, Server!";
    client.sendData(data);
	return client;
}

// sends an order to the server and receives the laptop (or laptop information) from the server.
LaptopInfo OrderLaptop(const Order& order, Client& client) {
	// Convert the order details to a string representation
    std::string orderData = order.serialize();

    // Send the order data to the server
    client.sendData(orderData);

    // Receive the response from the server
    char buffer[1024]; // Assuming a reasonable buffer size
    int bytesRead = recv(client.getSocket(), buffer, sizeof(buffer), 0);

    if (bytesRead < 0) {
        std::cerr << "Error receiving data from the server" << std::endl;
        exit(1);
    }

    // Unmarshal the received data to get the LaptopInfo
    std::string receivedData(buffer, bytesRead); // Assuming the received data is a string
    LaptopInfo laptopInfo;
    laptopInfo.deserialize(receivedData);
	return laptopInfo;
}

int main(int argc, char *argv[]) {

    Client client = init("127.0.0.1", 8080);
	Order order = Order(1,1,2);
	LaptopInfo laptopInfo = OrderLaptop(order, client);
	std::cout << "LaptopInfo received from Server:\n" << laptopInfo.toString() << std::endl;
    
    // if (argc != 6) {
    //     std::cerr << "Usage: ./client [ip addr] [port #] [# customers] [# orders] [laptop type]" << std::endl;
    //     return 1;
    // }

    

    // std::string ip = argv[1];
    // int port = std::stoi(argv[2]);
    // int numCustomers = std::stoi(argv[3]);
    // int numOrdersPerCustomer = std::stoi(argv[4]);
    // int laptopType = std::stoi(argv[5]);

    // std::vector<std::thread> customerThreads;
    // std::vector<Customer> customers;

    // // Create threads for each customer
    // for(int i = 0; i < numCustomers; i++) {
    //     customers.emplace_back(i+1, ip, port, numOrdersPerCustomer, laptopType);
    //     customerThreads.emplace_back(&Customer::run, &customers.back());
    // }

    // // Wait for all threads to finish
    // for(auto& thread : customerThreads) {
    //     thread.join();
    // }

	return 1;
}

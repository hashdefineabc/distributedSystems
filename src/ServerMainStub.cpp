#include <iostream>
#include "ServerStub.cpp"  // Include the ServerStub class
#include "serialization.h" // Include the serialization functions and Order/LaptopInfo structures

int main(int argc, char* argv[]) {

    if (argc < 2)
    {
        std::cerr << "Usage\n"
                  << argv[0] << " [port]" << std::endl;
        return 0;
    }
    int serverPort = atoi(argv[1]);
    ServerStub serverStub(serverPort);

    while (true)
    {
        // Accept a client connection and receive an order
        Order receivedOrder = serverStub.ReceiveOrder();

        // Process the received order and assemble laptop information
        LaptopInfo laptopInfo;
        laptopInfo.customer_id = receivedOrder.customer_id;
        laptopInfo.order_number = receivedOrder.order_number;
        laptopInfo.laptop_type = receivedOrder.laptop_type;
        laptopInfo.engineer_id = 123; // Set engineer ID (for example)
        laptopInfo.expert_id = -1;    // No custom module

        // Ship the laptop information back to the client
        serverStub.ShipLaptop(laptopInfo);
    }

    return 1;
}

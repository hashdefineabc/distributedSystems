#include <iostream>
#include <thread>
#include "Serialization.h"
#include "Client.cpp"

struct LatencyStats {
    long long totalLatency = 0;
    long long minLatency = std::numeric_limits<long long>::max();
    long long maxLatency = std::numeric_limits<long long>::min();
    int numOrders = 0;
};

LaptopInfo OrderLaptop(const Order& order, Client &client) {
    // Marshal order into a byte stream and send it
    std::string orderData = MarshalOrder(order);
    client.sendData(orderData);

    // Receive laptop information in byte stream format and unmarshal it
    std::string laptopData = client.receiveData();
    LaptopInfo laptopInfo = UnmarshalLaptopInfo(laptopData);

    return laptopInfo;
}

void customerThread(int customerID, int numOrders, int laptopType, LatencyStats& latencyStats, std::unique_ptr<Client>& clientPtr) {
    Client& client = *clientPtr;
    for (int orderNumber = 1; orderNumber <= numOrders; ++orderNumber) {

        auto start = std::chrono::high_resolution_clock::now();

        // Create an Order object and populate its fields
        Order order;
        order.customer_id = customerID;
        order.order_number = orderNumber;
        order.laptop_type = laptopType;

        // Send the order to the server and receive laptop information
        LaptopInfo laptopInfo = OrderLaptop(order, client);

        // Handle the received laptop information
        //std::cout << "Customer " << laptopInfo.customer_id << " OrderNumber " << laptopInfo.order_number << " Info:" << std::endl;
        // std::cout << "Customer ID: " << laptopInfo.customer_id << std::endl;
        // std::cout << "Order Number: " << laptopInfo.order_number << std::endl;
        // std::cout << "Laptop Type: " << laptopInfo.laptop_type << std::endl;
        // std::cout << "Engineer ID: " << laptopInfo.engineer_id << std::endl;
        // std::cout << "Expert ID: " << laptopInfo.expert_id << "\n" << std::endl;

        auto end = std::chrono::high_resolution_clock::now();
        auto latency = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        latencyStats.totalLatency += latency;
        latencyStats.minLatency = std::min(latencyStats.minLatency, latency);
        latencyStats.maxLatency = std::max(latencyStats.maxLatency, latency);
        latencyStats.numOrders++;

    }
}


int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " [ip addr] [port #] [# customers] [# orders] [laptop type]" << std::endl;
        return 1;
    }

    const std::string serverIP = argv[1];
    int serverPort = atoi(argv[2]);
    int numCustomers = atoi(argv[3]);
    int numOrdersPerCustomer = atoi(argv[4]);
    int laptopType = atoi(argv[5]);

    std::unique_ptr<Client> clientPtr(new Client(serverIP, serverPort));


    LatencyStats latencyStats;
    std::vector<std::thread> customerThreads;

    for (int i = 1; i <= numCustomers; ++i) {
        customerThreads.emplace_back(customerThread, i, numOrdersPerCustomer, laptopType, std::ref(latencyStats), std::ref(clientPtr));
    }

    // Join all customer threads
    for (std::thread& thread : customerThreads) {
        thread.join();
    }

    // Calculate statistics
    double avgLatency = static_cast<double>(latencyStats.totalLatency) / latencyStats.numOrders;
    long long minLatency = latencyStats.minLatency;
    long long maxLatency = latencyStats.maxLatency;

    std::cout << "Average latency: " << avgLatency << " microseconds" << std::endl;
    std::cout << "Minimum Latency: " << minLatency << " microseconds" << std::endl;
    std::cout << "Maximum Latency: " << maxLatency << " microseconds" << std::endl;
    double throughput = static_cast<double>(latencyStats.numOrders) / (latencyStats.totalLatency/1000);

    std::cout << "Throughput: " << throughput << " orders/second" << std::endl;

    return 0;
}

#include <iostream>
#include <thread>
#include "Order.h"
#include "LaptopInfo.h"
#include "ClientStub.cpp"

struct LatencyStats {
    long long totalLatency = 0;
    long long minLatency = std::numeric_limits<long long>::max();
    long long maxLatency = std::numeric_limits<long long>::min();
    int numOrders = 0;
};

void customerThread(int customerID, int numOrders, int laptopType, LatencyStats& latencyStats, ClientStub clientStub) {
    
    for (int orderNumber = 1; orderNumber <= numOrders; ++orderNumber) {

        auto start = std::chrono::high_resolution_clock::now();

        // Create an Order object and populate its fields
        Order order;
        order.customer_id = customerID;
        order.order_number = orderNumber;
        order.laptop_type = laptopType;

        LaptopInfo laptopInfo;

        try {
            // Send the order to the server and receive laptop information
            laptopInfo = clientStub.OrderLaptop(order);
        }
        catch (std::exception e) {
            std::cerr << "CustomerId " << customerID << " order number " << orderNumber << " failed "  << std::endl;
            break;
        }

        

        // Handle the received laptop information
        //std::cout << "Customer " << laptopInfo.customer_id << " OrderNumber " << laptopInfo.order_number << " Info:" << std::endl;
        // std::cout << "Customer ID: " << laptopInfo.customer_id << std::endl;
        // std::cout << "Order Number: " << laptopInfo.order_number << std::endl;
        // std::cout << "Laptop Type: " << laptopInfo.laptop_type << std::endl;
        // std::cout << "Engineer ID: " << laptopInfo.engineer_id << std::endl;
        // std::cout << "Expert ID: " << laptopInfo.expert_id << "\n" << std::endl;

        auto end = std::chrono::high_resolution_clock::now();
        auto latency = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "Customer " << customerID << " received Laptop - Type: " << laptopInfo.laptop_type << " OrderNumber: " << laptopInfo.order_number << " EngineerId: " << laptopInfo.engineer_id << " ExpertId: " << laptopInfo.expert_id << std::endl;
        
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

    LatencyStats latencyStats;
    std::vector<std::thread> customerThreads;

    for (int i = 1; i <= numCustomers; ++i) {
        ClientStub clientStub(serverIP, serverPort);
        customerThreads.emplace_back(customerThread, i, numOrdersPerCustomer, laptopType, std::ref(latencyStats), std::move(clientStub));
    }

    // Join all customer threads
    for (std::thread& thread : customerThreads) {
        thread.join();
    }

    // Calculate statistics
    double avgLatency = static_cast<double>(latencyStats.totalLatency) / latencyStats.numOrders;
    long long minLatency = latencyStats.minLatency;
    long long maxLatency = latencyStats.maxLatency;

    std::cout << "\nTotal Number of Orders: " << latencyStats.numOrders << std::endl;
    std::cout << "\nAverage latency: " << avgLatency << " microseconds" << std::endl;
    std::cout << "Minimum Latency: " << minLatency << " microseconds" << std::endl;
    std::cout << "Maximum Latency: " << maxLatency << " microseconds" << std::endl;
    double throughput = static_cast<double>(latencyStats.numOrders) / (latencyStats.totalLatency/1000);

    std::cout << "Throughput: " << throughput << " orders/second" << std::endl;

    return 1;
}

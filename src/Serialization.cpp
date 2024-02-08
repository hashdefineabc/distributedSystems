#include "Serialization.h"

// Function to marshal an Order structure into a byte stream
std::string MarshalOrder(const Order& order) {
    std::string data;
    data.append(reinterpret_cast<const char*>(&order.customer_id), sizeof(order.customer_id));
    data.append(reinterpret_cast<const char*>(&order.order_number), sizeof(order.order_number));
    data.append(reinterpret_cast<const char*>(&order.laptop_type), sizeof(order.laptop_type));
    return data;
}

// Function to unmarshal a byte stream into an Order structure
Order UnmarshalOrder(const std::string& data) {
    Order order;
    std::memcpy(&order.customer_id, data.c_str(), sizeof(order.customer_id));
    std::memcpy(&order.order_number, data.c_str() + sizeof(order.customer_id), sizeof(order.order_number));
    std::memcpy(&order.laptop_type, data.c_str() + sizeof(order.customer_id) + sizeof(order.order_number), sizeof(order.laptop_type));
    return order;
}

// Function to marshal a LaptopInfo structure into a byte stream
std::string MarshalLaptopInfo(const LaptopInfo& laptopInfo) {
    std::string data;
    data.append(reinterpret_cast<const char*>(&laptopInfo.customer_id), sizeof(laptopInfo.customer_id));
    data.append(reinterpret_cast<const char*>(&laptopInfo.order_number), sizeof(laptopInfo.order_number));
    data.append(reinterpret_cast<const char*>(&laptopInfo.laptop_type), sizeof(laptopInfo.laptop_type));
    data.append(reinterpret_cast<const char*>(&laptopInfo.engineer_id), sizeof(laptopInfo.engineer_id));
    data.append(reinterpret_cast<const char*>(&laptopInfo.expert_id), sizeof(laptopInfo.expert_id));
    return data;
}

// Function to unmarshal a byte stream into a LaptopInfo structure
LaptopInfo UnmarshalLaptopInfo(const std::string& data) {
    LaptopInfo laptopInfo;
    std::memcpy(&laptopInfo.customer_id, data.c_str(), sizeof(laptopInfo.customer_id));
    std::memcpy(&laptopInfo.order_number, data.c_str() + sizeof(laptopInfo.customer_id), sizeof(laptopInfo.order_number));
    std::memcpy(&laptopInfo.laptop_type, data.c_str() + sizeof(laptopInfo.customer_id) + sizeof(laptopInfo.order_number), sizeof(laptopInfo.laptop_type));
    std::memcpy(&laptopInfo.engineer_id, data.c_str() + sizeof(laptopInfo.customer_id) + sizeof(laptopInfo.order_number) + sizeof(laptopInfo.laptop_type), sizeof(laptopInfo.engineer_id));
    std::memcpy(&laptopInfo.expert_id, data.c_str() + sizeof(laptopInfo.customer_id) + sizeof(laptopInfo.order_number) + sizeof(laptopInfo.laptop_type) + sizeof(laptopInfo.engineer_id), sizeof(laptopInfo.expert_id));
    return laptopInfo;
}
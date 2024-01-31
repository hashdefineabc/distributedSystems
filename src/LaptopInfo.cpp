#include <iostream>
#include <sstream>

class LaptopInfo {
    private: 
        int customer_id; // copied from the order
        int order_number; // copied from the order
        int laptop_type; // copied from the order
        int engineer_id; // id of the engineer who created the laptop 
        int expert_id; // id of the expert who added a custom module 
                        // -1 indicates that there is no custom module
    public:
    // Default constructor
    LaptopInfo(): 
        customer_id(0), order_number(0), laptop_type(0), engineer_id(0), expert_id(0) {}

    // Parameterized constructor
    LaptopInfo(int customer_id, int order_number, int laptop_type, int engineer, int expert_id): 
        customer_id(customer_id), order_number(order_number), laptop_type(laptop_type), engineer_id(engineer), expert_id(expert_id) {};

     // Provide a string representation of the laptop information (for simplicity)
    std::string toString() const {
        return "Customer ID: " + std::to_string(customer_id) +
               "\nOrder Number: " + std::to_string(order_number) +
               "\nLaptop Type: " + std::to_string(laptop_type) +
               "\nEngineer ID: " + std::to_string(engineer_id) +
               "\nExpert ID: " + std::to_string(expert_id);
    }

    // Serialize method for converting a LaptopInfo object to a string
    std::string serialize() const {
        // This is a simple example; adjust it based on your actual serialization format
        std::ostringstream oss;
        oss << customer_id << ' ' << order_number << ' ' << laptop_type << ' ' << engineer_id << ' ' << expert_id;
        return oss.str();
    }

    // Deserialize a string representation into a LaptopInfo object
    void deserialize(const std::string& data) {
        // This is a simple example; you might need to implement a more robust deserialization logic
        std::istringstream iss(data);
        iss >> customer_id >> order_number >> laptop_type >> engineer_id >> expert_id;
    }
};
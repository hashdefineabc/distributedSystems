#include <iostream>
#include <sstream>

class Order {
    private:
        int customer_id; // customer id
        int order_number; // # of orders issued by this customer so far
        int laptop_type; // either 0 - regular or 1 - custom

    public:
    // Default constructor
    Order() : customer_id(0), order_number(0), laptop_type(0) {}
    // Parameterized constructor
    Order(int customer_id, int order_number, int laptop_type)
        : customer_id(customer_id), order_number(order_number), laptop_type(laptop_type) {}

    int getCustomerId() const { return customer_id; }
    int getOrderNumber() const { return order_number; }
    int getLaptopType() const { return laptop_type; }

    // Provide a string representation of the order (for simplicity)
    std::string toString() const {
        return "Customer ID: " + std::to_string(customer_id) +
               "\nOrder Number: " + std::to_string(order_number) +
               "\nLaptop Type: " + std::to_string(laptop_type);
    }

    // Serialize method for converting a LaptopInfo object to a string
    std::string serialize() const {
        // This is a simple example; adjust it based on your actual serialization format
        std::ostringstream oss;
        oss << customer_id << ' ' << order_number << ' ' << laptop_type << ' ';
        return oss.str();
    }

    // Deserialize method for converting a string to an Order object
    void deserialize(const std::string& data) {
        // This is a simple example; adjust it based on your actual data format
        std::istringstream iss(data);
        iss >> customer_id >> order_number >> laptop_type;
    }
     
};
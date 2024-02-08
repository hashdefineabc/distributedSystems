// Laptop information structure
struct LaptopInfo {
    int customer_id; // from order
    int order_number; // from order
    int laptop_type;  // from order
    int engineer_id; // id of the engineer who created the laptop
    int expert_id; // id of the expert who added a custom module
                   // -1 indicates that there is no custom module

    LaptopInfo() : customer_id(0), order_number(0), laptop_type(0), engineer_id(0), expert_id(0) {}

    LaptopInfo(int customer_id, int order_number, int laptop_type, int engineer_id, int expert_id) : customer_id(customer_id), order_number(order_number), laptop_type(laptop_type), engineer_id(engineer_id), expert_id(expert_id) {}

    std::string MarshalLaptopInfo() const { 
        std::stringstream strstream;
        strstream << customer_id << " " << order_number << " " << laptop_type << " " << engineer_id << " " << expert_id;
        return strstream.str();
    }

    static LaptopInfo UnmarshalLaptopInfo(const std::string& s) {
        std::stringstream strstream(s);
        int customer_id, order_number, laptop_type, engineer_id, expert_id;
        strstream >> customer_id >> order_number >> laptop_type >> engineer_id >> expert_id;

        return LaptopInfo(customer_id, order_number, laptop_type, engineer_id, expert_id);
    }
};

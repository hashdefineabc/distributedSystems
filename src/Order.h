#ifndef ORDER_H
#define ORDER_H
#include <sstream>

// Order structure
struct Order {
    int customer_id; // customer id
    int order_number; // order number
    int laptop_type; // 0 for regular, 1 for custom

    // default constructor
    Order() : customer_id(0), order_number(0), laptop_type(0) {}

    Order(int customer_id, int order_number, int laptop_type) : customer_id(customer_id), order_number(order_number), laptop_type(laptop_type) {}

    std::string MarshalOrder() const {
        std::stringstream order;
        order << customer_id << " " << order_number << " " << laptop_type;
        return order.str();
    }

    static Order UnmarshalOrder(const std::string& order) {
        std::stringstream ss(order);
        int customer_id, order_number, laptop_type;
        ss >> customer_id >> order_number >> laptop_type;
        return Order(customer_id, order_number, laptop_type);
    }
};
#endif // ORDER_H
#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <string>
#include "LaptopInfo.h"
#include "Order.h"

std::string MarshalOrder(const Order& order);
Order UnmarshalOrder(const std::string& data);
std::string MarshalLaptopInfo(const LaptopInfo& laptopInfo);
LaptopInfo UnmarshalLaptopInfo(const std::string& data);

#endif
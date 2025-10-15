// File: src/record.cpp
//
// The Record class represents a key-value pair
// where the value can be of multiple types (int, double, or string).

#include "../include/record.hpp"
#include <sstream>

Record::Record(const std::string&k, const ValueType& v)
    : key(k), value(v) {}

std::string Record::getValueAsString() const {
    std::ostringstream oss;

    // Convert the variant value to string
    std::visit([&oss](auto&& arg) {
        oss << arg;
    }, value);

    return oss.str();
}

void Record::print() const {
    std::cout << "Key: " << key << " | Value: ";

    // Print the value based on its actual type
    std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr(std::is_same_v<T, int>) {
            std::cout << arg << " (int)";
        } else if constexpr(std::is_same_v<T, double>) {
            std::cout << arg << " (double)";
        } else if constexpr(std::is_same_v<T, std::string>) {
            std::cout << "\"" << arg << "\" (string)";
        }
    }, value);

    std::cout << std::endl;
}

// Compare Record with a string key
bool Record::operator==(const std::string& k) const {
    return key == k;
}
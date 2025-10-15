#pragma once
#include <string>
#include <variant>
#include <iostream>

class Record {
public:
    using ValueType = std::variant<int, double, std::string>;

    std::string key;
    ValueType value;

    Record() = default;
    Record(const std::string& k, const ValueType& v);

    std::string getValueAsString() const;
    void print() const;

    bool operator==(const std::string& k) const;
};
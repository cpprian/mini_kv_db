#include "../include/database.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Database::Database(const std::string& path) : filepath(path) {
    load();
}

std::vector<Record>::iterator Database::findRecord(const std::string& key) {
    return std::find_if(records.begin(), records.end(),
        [&key](const Record& r) { return r.key == key; });
}

std::vector<Record>::const_iterator Database::findRecord(const std::string& key) const {
    return std::find_if(records.begin(), records.end(),
        [&key](const Record& r) { return r.key == key; });
}

void Database::addRecord(const Record& rec) {
    auto it = findRecord(rec.key);
    if (it != records.end()) {
        std::cout << "Warning: Key '" << rec.key << "' already exists. Use update to modify.\n";
        return;
    }

    records.push_back(rec);
    std::cout << "Records added successfully.\n";
}

bool Database::updateRecord(const std::string& key, const Record::ValueType& newValue) {
    auto it = findRecord(key);
    if (it == records.end()) {
        return false;
    }
    it->value = newValue;
    return true;
}

bool Database::deleteRecord(const std::string& key) {
    auto it = findRecord(key);
    if (it == records.end()) {
        return false;
    }
    records.erase(it);
    return true;
}

std::optional<Record> Database::getRecord(const std::string& key) const {
    auto it = findRecord(key);
    if (it == records.end()) {
        return std::nullopt;
    }
    return *it;
}


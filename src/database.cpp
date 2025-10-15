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

void Database::save() const {
    json j;

    for (const auto& r : records) {
        if (std::holds_alternative<int>(r.value)) {
            j[r.key] = std::get<int>(r.value);
        } else if (std::holds_alternative<double>(r.value)) {
            j[r.key] = std::get<double>(r.value);
        } else if (std::holds_alternative<std::string>(r.value)) {
            j[r.key] = std::get<std::string>(r.value);
        }
    }

    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filepath << std::endl;
        return;
    }

    file << j.dump(4);
    file.close();

    std::cout << "Database saved to " << filepath << " (" << records.size() << " records)\n";
}

void Database::load() {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "No existing database found. Starting fresh.\n";
        return;
    }

    try {
        json j;
        file >> j;
        records.clear();

        for (auto it = j.begin(); it != j.end(); it++) {
            std::string key = it.key();

            if (it->is_string()) {
                records.emplace_back(key, it->get<std::string>());
            } else if (it->is_number_integer()) {
                records.emplace_back(key, it->get<int>());
            } else if (it->is_number_float()) {
                records.emplace_back(key, it->get<double>());
            }
        }
    } catch(const json::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        records.clear();
    }

    file.close();
}

void Database::listAll() const {
    if (records.empty()) {
        std::cout << "Database is empty.\n";
        return;
    }

    std::cout << "\n=== Database Contents (" << records.size() << " records) ===\n";
    for (const auto& rec : records) {
        rec.print();
    }
    std::cout << "================================\n\n";
}

std::vector<Record> Database::search(const std::string& pattern) const {
    std::vector<Record> results;

    for (const auto& rec : records) {
        if (rec.key.find(pattern) != std::string::npos) {
            results.push_back(rec);
        }
    }

    return results;
}

void Database::clear() {
    records.clear();
    std::cout << "Database cleared.\n";
}

void Database::sortByKey(bool ascending) {
    if (ascending) {
        std::sort(records.begin(), records.end(),
            [](const Record& a, const Record& b) { return a.key < b.key; });
    } else {
        std::sort(records.begin(), records.end(),
            [](const Record& a, const Record& b) { return a.key > b.key; });
    }
    std::cout << "Records sorted by key ("
                << (ascending ? "ascending" : "descending") << ").\n";
}

void Database::printStatistics() const {
    int intCount = 0, doubleCount = 0, stringCount = 0;

    for (const auto& rec : records) {
        if (std::holds_alternative<int>(rec.value)) {
            intCount++;
        } else if (std::holds_alternative<double>(rec.value)) {
            doubleCount++;
        } else if (std::holds_alternative<std::string>(rec.value)) {
            stringCount++;
        }
    }
    
    std::cout << "\n=== Database Statistics ===\n";
    std::cout << "Total records: " << records.size() << "\n";
    std::cout << "  - Integer values: " << intCount << "\n";
    std::cout << "  - Double values: " << doubleCount << "\n";
    std::cout << "  - String values: " << stringCount << "\n";
    std::cout << "===========================\n\n";
}
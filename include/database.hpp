#pragma oncde
#include "record.hpp"
#include <vector>
#include <optional>
#include <memory>

// Database class to manage records
class Database {
private:
    std::vector<Record> records;
    std::string filepath;

    // helpers
    std::vector<Record>::iterator findRecord(const std::string& key);
    std::vector<Record>::const_iterator findRecord(const std::string& key) const;

public:
    explicit Database(const std::string& path);

    // crud
    void addRecord(const Record& rec);
    bool updateRecord(const std::string& key, const Record::ValueType& newValue);
    bool deleteRecord(const std::string& key);
    std::optional<Record> getRecord(const std::string& key) const;

    // display
    void listAll() const;
    size_t size() const { return records.size(); }

    // search
    std::vector<Record> search(const std::string& pattern) const;

    // sorting
    void sortByKey(bool ascending = true);

    // filtering
    template<typename T>
    std::vector<Record> filterByType() const {
        std::vector<Record> results;

        for (const auto& rec : records) {
            if (std::holds_alternative<T>(rec.value)) {
                results.push_back(rec);
            }
        }
        return results;
    }

    // stats
    void printStatistics() const;

    // DANGER: delete all values
    void clear();
};
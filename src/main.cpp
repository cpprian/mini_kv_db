// File: src/main.cpp
//
// A simple command-line interface for the mini key-value database.
// Supports adding, retrieving, updating, deleting, listing, searching records,
// displaying statistics, clearing the database.
// There is also an interactive mode for user-friendly operations.

#include "../include/database.hpp"
#include <iostream>
#include <string>
#include <limits>

void printHelp(const char* progName) {
    std::cout << "Mini Key-Value Database\n\n";
    std::cout << "Usage:\n";
    std::cout << "  " << progName << " add <key> <value>      - Add a new record\n";
    std::cout << "  " << progName << " get <key>              - Get a record by key\n";
    std::cout << "  " << progName << " update <key> <value>   - Update existing record\n";
    std::cout << "  " << progName << " delete <key>           - Delete a record\n";
    std::cout << "  " << progName << " list                   - List all records\n";
    std::cout << "  " << progName << " search <pattern>       - Search keys by pattern\n";
    std::cout << "  " << progName << " clear                  - Clear all records\n";
    std::cout << "  " << progName << " stats                  - Display statistics\n";
    std::cout << "  " << progName << " interactive            - Start interactive mode\n";
    std::cout << "  " << progName << " --help                 - Show this help\n\n";
    std::cout << "Database file: data/db.json (default)\n";
    std::cout << "Set DB_PATH environment variable to use custom path\n";
}

void printMenu() {
    std::cout << "\n=== Mini Key-Value Database ===\n";
    std::cout << "1. Add record\n";
    std::cout << "2. Get record\n";
    std::cout << "3. Update record\n";
    std::cout << "4. Delete record\n";
    std::cout << "5. List all records\n";
    std::cout << "6. Search by key pattern\n";
    std::cout << "7. Save database\n";
    std::cout << "8. Display staistics\n";
    std::cout << "9. Clear database\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

Record::ValueType parseValue(const std::string& val) {
    try {
        size_t pos;
        int intVal = std::stoi(val, &pos);
        if (pos == val.length()) {
            return intVal;
        }
    } catch(...) {}

    try {
        size_t pos;
        double dblVal = std::stod(val, &pos);
        if (pos == val.length()) {
            return dblVal;
        }
    } catch(...) {}

    return val;
}

Record::ValueType getValueFromUser() {
    std::cout << "Value type (1=int, 2=double, 3=string, auto=detect): ";
    std::string typeChoice;
    std::getline(std::cin, typeChoice);

    if (typeChoice == "1") {
        std::cout << "Enter int value: ";
        int val;
        
        std::cin >> val;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return val;
    } else if (typeChoice == "2") {
        std::cout << "Enter double value: ";
        double val;

        std::cin >> val;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return val;
    } else {
        std::cout << "Enter value: ";
        std::string val;

        std::getline(std::cin, val);

        if (typeChoice == "3") {
            return val;
        } else {
            // auto-detect
            return parseValue(val);
        }
    }
}

void interactiveMode(Database& db) {
    int choice;
    bool running = true;

    while (running) {
        printMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            std::cout << "Enter key: ";
            std::string key;
            std::getline(std::cin, key);

            auto value = getValueFromUser();
            db.addRecord(Record(key, value));
            break;
        }
        case 2: {
            std::cout << "Enter key: ";
            std::string key;
            std::getline(std::cin, key);

            auto rec = db.getRecord(key);
            if (rec.has_value()) {
                std::cout << "Found: ";
                rec->print();
            } else {
                std::cout << "Record not found.\n";
            }
            break;
        }
        case 3: {
            std::cout << "Enter key to update: ";
            std::string key;
            std::getline(std::cin, key);

            auto value = getValueFromUser();
            if (db.updateRecord(key, value)) {
                std::cout << "Record updated successfully.\n";
            } else {
                std::cout << "Record not found.\n";
            }
            break;
        }
        case 4: {
            std::cout << "Enter key to delete: ";
            std::string key;
            std::getline(std::cin, key);

            if (db.deleteRecord(key)) {
                std::cout << "Record deleted successfully.\n";
            } else {
                std::cout << "Record not found.\n";
            }
            break;
        }
        case 5: {
            db.listAll();
            break;
        }
        case 6: {
            std::cout << "Enter search pattern: ";
            std::string pattern;
            std::getline(std::cin, pattern);

            auto results = db.search(pattern);
            if (results.empty()) {
                std::cout << "No matching records found.\n";
            } else {
                std::cout << "\n=== Search Results (" << results.size() << ") ===\n";
                for (const auto& rec : results) {
                    rec.print();
                }
                std::cout << "================================\n";
            }
            break;
        }
        case 7: {
            db.save();
            break;
        }
        case 8: {
            db.printStatistics();
            break;
        }
        case 9: {
            std::cout << "Are you sure? (y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (confirm == 'y' || confirm == 'Y') {
                db.clear();
            }
            break;
        }
        case 0: {
            std::cout << "Save before exit? (y/n): ";
            char save;
            std::cin >> save;
            if (save == 'y' || save == 'Y') {
                db.save();
            }
            std::cout << "Goodbye!\n";
            running = false;
            break;
        }
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}

int main(int argc, char* argv[]) {
    const char* dbPathEnv = std::getenv("DB_PATH");
    std::string dbPath = dbPathEnv ? dbPathEnv : "data/db.json";

    Database db(dbPath);

    if (argc < 2 || (argc == 2 && std::string(argv[1]) == "interactive")) {
        interactiveMode(db);
        return 0;
    }

    std::string command = argv[1];

    if (command == "--help" || command == "-h") {
        printHelp(argv[0]);
        return 0;
    }

    if (command == "add" && argc == 4) {
        std::string key = argv[2];
        std::string val = argv[3];

        auto value = parseValue(val);
        db.addRecord(Record(key, value));
        db.save();
        std::cout << "Record added.\n";
    } else if (command == "get" && argc == 3) {
        auto rec = db.getRecord(argv[2]);
        if (rec.has_value()) {
            std::cout << "Key: " << rec->key << " | Value: "
                            << rec->getValueAsString() << "\n";
        } else {
            std::cout << "Record not found.\n";
        }
    } else if (command == "update" && argc == 4) {
        std::string key = argv[2];
        std::string val = argv[3];

        auto value = parseValue(val);
        if (db.updateRecord(key, value)) {
            db.save();
            std::cout << "Record updated.\n";
        } else {
            std::cout << "Record not found.\n";
        }
    } else if (command == "delete" && argc == 3) {
        if (db.deleteRecord(argv[2])) {
            db.save();
            std::cout << "Record deleted.\n";
        } else {
            std::cout << "Record not found.\n";
        }
    } else if (command == "list") {
        db.listAll();
    } else if (command == "search" && argc == 3) {
        auto results = db.search(argv[2]);

        if (results.empty()) {
            std::cout << "No matching records found.\n";
        } else {
            std::cout << "Found " << results.size() << " record(s): \n";
            for (const auto& rec : results) {
                rec.print();
            }
        }
    } else if (command == "stats") {
        db.printStatistics();
    } else if (command == "clear") {
        std::cout << "Are you sure you want to clear the database? (yes/no): ";
        std::string confirm;
        std::cin >> confirm;

        if (confirm == "yes") {
            db.clear();
            db.save();
            std::cout << "Database cleared.\n";
        } else {
            std::cout << "Operation cancelled.\n";
        }
    } else {
        std::cerr << "Invalid command or arguments.\n";
        printHelp(argv[0]);
        return 1;
    }

    return 0;
}
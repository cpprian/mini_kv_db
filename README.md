# Mini Key-Value Database

A simple key-value database written in C++17, CMake and nlohmann/json.
It supports basic operations like `put`, `get`, and `delete`, and stores data in a JSON file.

## Features

- Supports multiple data types: `int`, `double`, `string`.
- `std::variant` is used for storing different data types.
- A classical CRUD interface with `add`, `get`, `update`, and `delete` operations.
- It stores data in a JSON file for persistence.
- Displaying sorting and filtering of key-value pairs.
- Displaying statistics about the stored data.

## Requirements

- C++17 compatible compiler
- CMake 3.10 or higher
- Internet connection to download nlohmann/json library

## Building the Project

1. Clone the repository
2. Create a build directory and navigate into it:

   ```bash
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:

   ```bash
   cmake ..
   ```

4. Build the project:

   ```bash
   cmake --build .
   ```

## Example Usage

```bash
./mini_kv_db add username "cpprian"
./mini_kv_db add age 25
./mini_kv_db add balance 1234.56

# Read a record
./mini_kv_db get username

# Update a record
./mini_kv_db update age 26

# Search for a record
./mini_kv_db search user

# Display all records
./mini_kv_db list

# Delete a record
./mini_kv_db delete balance

# Clear the database
./mini_kv_db clear

# Display statistics
./mini_kv_db stats
```

### Interactive Mode

You can also run the database in interactive mode:

```bash
./mini_kv_db interactive

# Or simply
./mini_kv_db
```

## Todo List

- [ ] Transaction support (ACID properties)
- [ ] Concurrency control
- [ ] Indexing for faster searches
- [ ] Backup and restore functionality
- [ ] Encryption for data security
- [ ] Network support for remote access (TCP/HTTP server)
- [ ] Unit tests and integration tests (Google Test)
- [ ] Docker support
- [ ] Benchmarking 
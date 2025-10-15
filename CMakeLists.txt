cmake_minimum_required(VERSION 3.14)
project(MiniKVDatabase VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/database.cpp
    src/record.cpp
)

add_executable(mini_kv_db ${SOURCES})

target_link_libraries(mini_kv_db PRIVATE nlohmann_json::nlohmann_json)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
endif()

install(TARGETS mini_kv_db DESTINATION bin)
cmake_minimum_required(VERSION 3.15)
project(AuthServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH ./vendors)

add_executable(${PROJECT_NAME} main.cpp)

# pqxx
find_package(libpqxx REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE libpqxx::pqxx)

# httplib
find_package(httplib REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib)

# nlohmann_json
find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# cryptopp
find_package(cryptopp REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE cryptopp::cryptopp)
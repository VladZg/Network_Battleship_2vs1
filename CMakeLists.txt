cmake_minimum_required(VERSION 3.5)

project(Network_Battleship_2vs1 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

add_executable(server ./Source/Server.cpp)
add_executable(client ./Source/Client.cpp)

target_link_libraries(server Qt5::Core Qt5::Network)
target_link_libraries(client Qt5::Core Qt5::Network)

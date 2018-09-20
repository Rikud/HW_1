cmake_minimum_required(VERSION 3.10)
project(HW_1)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.58.0 REQUIRED system)
include_directories(include)

add_executable(HW_1 src/main.cpp src/ConfReader.cpp headers/ConfReader.h src/MyServer.cpp headers/MyServer.h src/Connector.cpp headers/Connector.h src/Request.cpp headers/Request.h src/Response.cpp headers/Response.h)
target_link_libraries(HW_1 -pthread -lboost_thread -lboost_filesystem ${Boost_LIBRARIES})
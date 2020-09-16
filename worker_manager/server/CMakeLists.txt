cmake_minimum_required(VERSION 3.10)
project(old VERSION 1.0.0 DESCRIPTION "")
set(CMAKE_CXX_STANDARD 17)

include_directories(include src)
#add_executable(worker_manager_server src/worker_manager_server.cpp)
#target_link_libraries(worker_manager_server -lpthread -lbasket -lrpc  -lboost_system -lboost_filesystem -lmpi)
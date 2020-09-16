cmake_minimum_required(VERSION 3.10)
project(Rhea)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(Daemon SHARED include/Daemon.h src/Daemon.cpp)
target_link_libraries(Daemon -lmpi -lpthread)
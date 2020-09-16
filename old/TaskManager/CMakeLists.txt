cmake_minimum_required(VERSION 3.10)
project(Rhea)

find_package(MPI)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/queue/include)
include_directories(${CMAKE_SOURCE_DIR}/tasks/include)
include_directories(${CMAKE_SOURCE_DIR}/jobManager/include)
include_directories(${CMAKE_SOURCE_DIR}/basketCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/JobUtils/include)
link_directories(${CMAKE_BINARY_DIR}/jobManager)
link_directories(${CMAKE_BINARY_DIR}/basketCommon)
link_directories(${CMAKE_BINARY_DIR}/tasks)
link_directories(${CMAKE_BINARY_DIR}/queue)
include_directories(${CMAKE_SOURCE_DIR}/JobUtils)

add_executable(TaskManager main.cpp
        include/client/Client.h src/client/Client.cpp
        include/server/Server.h src/server/Server.cpp
        include/server/TaskExecutor.h src/server/TaskExecutor.cpp)


target_link_libraries(TaskManager -lmpi -ljob -lpthread -lcommon -lqueue) #TODO: REVIEW
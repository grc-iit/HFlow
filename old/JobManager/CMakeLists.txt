cmake_minimum_required(VERSION 3.10)
project(Rhea)

find_package(MPI)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/Job/include)
include_directories(${CMAKE_SOURCE_DIR}/basketCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/jobCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/queue/include)
link_directories(${CMAKE_BINARY_DIR}/Job)
link_directories(${CMAKE_BINARY_DIR}/basketCommon)
link_directories(${CMAKE_BINARY_DIR}/jobCommon)
link_directories(${CMAKE_BINARY_DIR}/queue)

add_executable(JobManager main.cpp
        include/client/Client.h src/client/Client.cpp
        include/server/Server.h src/server/Server.cpp
        include/server/FunctionManager.h src/server/FunctionManager.cpp
        include/dataStructures/ClusterConfig.h
        include/dataStructures/ClusterNode.h
        include/dataStructures/Job.h
        include/dataStructures/JobNode.h
        include/dataStructures/OperationType.h
        include/dataStructures/ResourceReport.h include/dataStructures/NodeLoad.h)

target_link_libraries(JobManager -lbasket -lmpi -lpthread -lrt -lrpc -lboost_filesystem -lbasketCommon)


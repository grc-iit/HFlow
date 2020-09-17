cmake_minimum_required(VERSION 3.10)
project(old VERSION 1.0.0 DESCRIPTION "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -g -DBASKET_ENABLE_RPCLIB")

add_subdirectory(scs_io_common)
include_directories(scs_io_common/include)

add_subdirectory(sentinel)
include_directories(sentinel/include)
add_dependencies(sentinel scs_io_common)

include_directories(src)
include_directories(include)

set(RHEA_COMMON include/rhea/common/configuration_manager.h
                include/rhea/common/data_structures.h
                include/rhea/common/enumerations.h
                include/rhea/common/error_codes.h)

set(RHEA_LIB ${RHEA_COMMON} src/rhea/lib/rhea.cpp include/rhea/rhea.h)
add_library(old SHARED ${RHEA_LIB})


set(RHEA_SERVER ${RHEA_COMMON} src/rhea/server/Server.cpp src/rhea/server/server.h src/rhea/server/main.cpp)

add_executable(rhea_server ${RHEA_SERVER})
target_link_libraries(rhea_server -pthread)

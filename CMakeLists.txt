cmake_minimum_required(VERSION 3.10)
project(rhea VERSION 1.0.0 DESCRIPTION "")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sentinel)
include_directories(sentinel/include)

add_subdirectory(scs_io_common)
include_directories(scs_io_common/include)

include_directories(src)
include_directories(include)

set(RHEA_COMMON include/rhea/common/configuration_manager.h include/rhea/common/data_structures.h include/rhea/common/enumerations.h include/rhea/common/error_codes.h)

set(RHEA_LIB ${RHEA_COMMON} src/rhea/lib/rhea.cpp include/rhea/rhea.h)
add_library(rhea SHARED ${RHEA_LIB})


set(RHEA_SERVER ${RHEA_COMMON} src/rhea/server/server.cpp src/rhea/server/server.h src/rhea/server/main.cpp)

add_executable(rhea_server ${RHEA_SERVER})
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(COMMON_BASKET include/basket/data_structure.h include/basket/configuration_manager.h include/basket/daemonize.h ../Daemon/src/Daemon.cpp ../Daemon/include/Daemon.h ../User/driverProgram.cpp)

add_library(basketCommon SHARED ${COMMON_BASKET})
set_target_properties(basketCommon PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(basketCommon -lbasket -lrpc)
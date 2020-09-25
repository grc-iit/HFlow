cmake_minimum_required(VERSION 3.10)
project(rhea VERSION 1.0.0 DESCRIPTION "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -g -DBASKET_ENABLE_RPCLIB") #TODO: FIXME: Enable -DENABLE_REDIS_IOCLIENT



#Debug flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMON_DEBUG_MSG")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMON_DEBUG_TIMER")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMON_DEBUG_TRACE")

add_subdirectory(scs_io_common)
include_directories(scs_io_common/include)



add_subdirectory(sentinel)
include_directories(sentinel/include)
add_dependencies(sentinel scs_io_common)

add_subdirectory(rhea_job)
add_dependencies(rhea_job sentinel)

include_directories(src)
include_directories(include)


set(LIBS -lpthread -lbasket -lrpc  -lboost_system -lboost_filesystem -lmpi -ldl -L${CMAKE_BINARY_DIR}/scs_io_common -lscs_io_common)



set(RHEA_COMMON ${CMAKE_SOURCE_DIR}/include/rhea/common/configuration_manager.h
        ${CMAKE_SOURCE_DIR}/include/rhea/common/data_structures.h
        ${CMAKE_SOURCE_DIR}/include/rhea/common/enumerations.h
        ${CMAKE_SOURCE_DIR}/include/rhea/common/error_codes.h ${CMAKE_SOURCE_DIR}/src/rhea/common/data_structures.cpp ${CMAKE_SOURCE_DIR}/include/rhea/common/typedefs.h)

set(RHEA_LIB ${RHEA_COMMON} src/rhea/lib/rhea.cpp include/rhea/rhea.h)
add_library(rhea SHARED ${RHEA_LIB})

set(RHEA_CLIENT_SERVICE ${RHEA_COMMON} src/rhea/client_service/rhea_client.cpp include/rhea/client_service/rhea_client.h src/rhea/client_service/main.cpp)
add_executable(rhea_client_service ${RHEA_CLIENT_SERVICE})
target_link_libraries(rhea_client_service ${LIBS})


set(RHEA_BYTEFLOW_REGULATOR ${RHEA_COMMON} ${CMAKE_SOURCE_DIR}/src/rhea/byte_flow_regulator/server.cpp ${CMAKE_SOURCE_DIR}/include/rhea/byte_flow_regulator/server.h)
add_executable(rhea_byteflow_regulator ${RHEA_BYTEFLOW_REGULATOR}  ${CMAKE_SOURCE_DIR}/src/rhea/byte_flow_regulator/main.cpp)
add_dependencies(rhea_byteflow_regulator sentinel)
target_link_libraries(rhea_byteflow_regulator ${LIBS} -L${CMAKE_BINARY_DIR}/sentinel -lsentinel)


set(RHEA_CLIENT ${RHEA_COMMON} ${CMAKE_SOURCE_DIR}/src/rhea/byte_flow_regulator/client.cpp ${CMAKE_SOURCE_DIR}/include/rhea/byte_flow_regulator/client.h ${CMAKE_SOURCE_DIR}/src/rhea/client_service/rhea_client.cpp ${CMAKE_SOURCE_DIR}/include/rhea/client_service/rhea_client.h ${CMAKE_SOURCE_DIR}/src/rhea/client_service/main.cpp)
add_library(rhea_client ${RHEA_CLIENT})
add_dependencies(rhea_client sentinel)
target_link_libraries(rhea_client ${LIBS} -L${CMAKE_BINARY_DIR}/sentinel -lsentinel)

add_subdirectory(test/integration)
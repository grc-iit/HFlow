cmake_minimum_required(VERSION 3.10)
project(rhea_job)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -g -DBASKET_ENABLE_RPCLIB -fPIC")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMON_DEBUG_TIMER")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/scs_io_common/include)
include_directories(${CMAKE_SOURCE_DIR}/sentinel/include)
include_directories(include)

set(RHEA_JOB_SRC include/rhea_job/common/rhea_job.h src/rhea_job.cpp)
add_library(rhea_job SHARED ${RHEA_JOB_SRC})
target_link_libraries(rhea_job -rdynamic -L${CMAKE_BINARY_DIR}/sentinel -lsentinel -L${CMAKE_BINARY_DIR} -lrhea)
set_target_properties(rhea_job PROPERTIES LINKER_LANGUAGE CXX)
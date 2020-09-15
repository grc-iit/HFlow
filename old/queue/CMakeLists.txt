cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/basketCommon/include)
link_directories(${CMAKE_BINARY_DIR}/basketCommon)

set(QUEUE_CLIENT_SRC src/client/client.cpp include/client/client.h)
set(QUEUE_SERVER_TASk_SRC src/server/server.cpp src/server/mainEvent.cpp include/server/server.h)
set(QUEUE_SERVER_EVENT_SRC src/server/server.cpp src/server/mainTask.cpp include/server/server.h)

add_library(queue SHARED ${QUEUE_CLIENT_SRC})
target_link_libraries(queue -lbasket -lbasketCommon) #name basketCommon

add_executable(queue_server_event ${QUEUE_SERVER_TASk_SRC})
add_executable(queue_server_task ${QUEUE_SERVER_EVENT_SRC})
target_link_libraries(queue_server_event -lbasket -lmpi -lpthread -lrt -lrpc -lboost_filesystem -lbasketCommon) #name basketCommon
target_link_libraries(queue_server_task -lbasket -lmpi -lpthread -lrt -lrpc -lboost_filesystem -lbasketCommon) #name basketCommon

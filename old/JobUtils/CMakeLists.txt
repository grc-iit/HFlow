cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)


include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/queue/include)
include_directories(${CMAKE_SOURCE_DIR}/tasks/include)
include_directories(${CMAKE_SOURCE_DIR}/jobManager/include)
include_directories(${CMAKE_SOURCE_DIR}/basketCommon/include)
link_directories(${CMAKE_BINARY_DIR}/jobManager)
link_directories(${CMAKE_BINARY_DIR}/tasks)
link_directories(${CMAKE_BINARY_DIR}/queue)
link_directories(${CMAKE_BINARY_DIR}/basketCommon)

set(COMMON_OP include/tasks/Task.h
        include/tasks/SourceTask.h src/tasks/SourceTask.cpp
        include/tasks/AggregateTask.h src/tasks/AggregateTask.cpp
        include/tasks/KeyByTask.h src/tasks/KeyByTask.cpp
        include/tasks/SinkTask.h src/tasks/SinkTask.cpp
        include/common/Timer.h include/events/Event.h
        include/events/Event.h include/common/Action.h)

add_library(JobUtils SHARED ${COMMON_OP})
target_link_libraries(JobUtils -lbasket -lrpc -lbasketCommon -lqueue)


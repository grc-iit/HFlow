cmake_minimum_required(VERSION 3.10)
project(Rhea)

set(CMAKE_CXX_STANDARD 17)

find_package(MPI)

add_subdirectory(TaskManager)
add_subdirectory(JobManager)
add_subdirectory(basketCommon)
add_subdirectory(JobUtils)
add_subdirectory(queue)
add_subdirectory(Daemon)


add_dependencies(queue basketCommon)
add_dependencies(queue_server_task basketCommon)
add_dependencies(queue_server_event basketCommon)
add_dependencies(JobUtils basketCommon)
add_dependencies(JobManager queue)
add_dependencies(JobManager TaskManager)
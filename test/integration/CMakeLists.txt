cmake_minimum_required(VERSION 3.10)
project(rhea_test)
set(CMAKE_CXX_STANDARD 17)

set(LIBS -lpthread -lbasket -lrpc  -lboost_system -lboost_filesystem -lmpi -ldl -L${CMAKE_BINARY_DIR}/scs_io_common -lscs_io_common)

#Jaime's original test case
add_executable(client_single single_write/test.cpp)
add_dependencies(client_single rhea sentinel)
target_link_libraries(client_single ${LIBS} rhea sentinel)

add_executable(client_multi multi_write/test.cpp)
add_dependencies(client_multi rhea sentinel)
target_link_libraries(client_multi ${LIBS} rhea sentinel)

set(test_files  rhea_client_test writeIO)
foreach (test_file ${test_files})
add_executable(${test_file} ${test_file}/test.cpp)
add_dependencies(${test_file} ${test_file} scs_io_common sentinel)
target_link_libraries(${test_file} ${LIBS} rhea scs_io_common sentinel)
endforeach ()

add_executable(byte_flow_regulator_test byte_flow_regulator/test.cpp)
add_dependencies(byte_flow_regulator_test rhea_client scs_io_common sentinel)
target_link_libraries(byte_flow_regulator_test ${LIBS} rhea_client scs_io_common sentinel)



add_executable(byte_flow_regulator_test_decision byte_flow_regulator/test_decision.cpp ${RHEA_BYTEFLOW_REGULATOR})
add_dependencies(byte_flow_regulator_test_decision rhea_client scs_io_common sentinel)
target_link_libraries(byte_flow_regulator_test_decision ${LIBS} rhea_client scs_io_common sentinel)


add_executable(job_manager_test job_manager/test.cpp ${CMAKE_SOURCE_DIR}/sentinel/src/sentinel/job_manager/server.cpp ${CMAKE_SOURCE_DIR}/sentinel/include/sentinel/job_manager/Server.h)
add_dependencies(job_manager_test rhea_client scs_io_common sentinel)
target_link_libraries(job_manager_test ${LIBS} sentinel)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMON_DEBUG_MSG")

add_executable(real_app_dynamic_1 real_apps/test1.cpp)
add_dependencies(real_app_dynamic_1 scs_io_common sentinel)
target_link_libraries(real_app_dynamic_1 ${LIBS} rhea scs_io_common sentinel)


add_executable(real_app_elastic_2 real_apps/test2.cpp)
add_dependencies(real_app_elastic_2 scs_io_common sentinel)
target_link_libraries(real_app_elastic_2 ${LIBS} rhea scs_io_common sentinel)



add_test(NAME rhea_client_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/rhea_client_test_exec)
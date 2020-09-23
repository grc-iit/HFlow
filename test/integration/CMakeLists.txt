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

add_executable(rhea_client_test_exec rhea/test.cpp)
add_dependencies(rhea_client_test_exec rhea scs_io_common sentinel)
target_link_libraries(rhea_client_test_exec ${LIBS} rhea scs_io_common sentinel)

add_executable(byte_flow_regulator_test_exec integration/byte_flow_regulator/test.cpp)
add_dependencies(byte_flow_regulator_test_exec rhea_client scs_io_common sentinel)
target_link_libraries(byte_flow_regulator_test_exec ${LIBS} rhea_client scs_io_common sentinel)


add_test(NAME rhea_client_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/rhea_client_test_exec)
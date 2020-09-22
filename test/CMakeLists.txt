cmake_minimum_required(VERSION 3.10)
project(rhea_test)
set(CMAKE_CXX_STANDARD 17)

set(LIBS -lpthread -lbasket -lrpc  -lboost_system -lboost_filesystem -lmpi -ldl -L${CMAKE_BINARY_DIR}/scs_io_common -lscs_io_common)

#Jaime's original test case
add_executable(client integration/old/main.cpp)
add_dependencies(client rhea sentinel)
target_link_libraries(client ${LIBS} rhea sentinel)

add_executable(rhea_client_test_exec integration/rhea/test.cpp)
add_dependencies(rhea_client_test_exec rhea scs_io_common sentinel)
target_link_libraries(rhea_client_test_exec ${LIBS} rhea scs_io_common sentinel)

add_test(NAME rhea_client_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/rhea_client_test_exec)
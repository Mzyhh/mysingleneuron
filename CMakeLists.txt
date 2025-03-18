cmake_minimum_required(VERSION 3.31.0)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(mysingleneuron C CXX)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
include_directories("/usr/include" )
link_libraries(m)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(CUnit REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.c)
file(GLOB_RECURSE MATRIX_TEST_FILES tests/matrix*.c)
file(GLOB_RECURSE TOKENIZE_TEST_FILES tests/tokenize*.c)
list(REMOVE_ITEM SRC_FILES "${CMAKE_SOURCE_DIR}/src/main.c")

message(STATUS "SRC_FILES: ${SRC_FILES}")
message(STATUS "MATRIX_TEST_FILES: ${MATRIX_TEST_FILES}")
message(STATUS "TOKENIZE_TEST_FILES: ${TOKENIZE_TEST_FILES}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

add_executable(playground src/main.c ${SRC_FILES})
target_include_directories(playground PRIVATE headers)

add_executable(matrix_test ${SRC_FILES} ${MATRIX_TEST_FILES})
target_include_directories(matrix_test PRIVATE headers)
target_link_libraries(matrix_test ${CUNIT_LIBRARIES})

add_executable(tokenize_test ${SRC_FILES} ${TOKENIZE_TEST_FILES})
target_include_directories(tokenize_test PRIVATE headers)
target_link_libraries(tokenize_test ${CUNIT_LIBRARIES})

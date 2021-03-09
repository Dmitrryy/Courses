cmake_minimum_required(VERSION 3.10)


project(Map C)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} main.c RBTree/src/RBTree.c RBTree/tests/utests.c RBTree/tests/utests.h RBTree/tests/calloc_test.c)
target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
target_link_options(${PROJECT_NAME} PRIVATE -lgcov)
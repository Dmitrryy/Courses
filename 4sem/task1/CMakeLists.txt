cmake_minimum_required(VERSION 3.17)


project(MIPT_Lunev C)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} main.c HashTable/HashTable.c)

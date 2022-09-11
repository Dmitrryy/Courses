
cmake_minimum_required(VERSION 3.9)

project(MPI_prog)

set(HELLO_EXEC hello)
add_executable(${HELLO_EXEC} Hello.cpp)

find_package(Threads REQUIRED)
target_link_libraries(${HELLO_EXEC} Threads::Threads)

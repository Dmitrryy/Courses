
cmake_minimum_required(VERSION 3.20)

project(MPI_lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")


set(HELLO_EXEC lab1)
add_executable(${HELLO_EXEC} main.cpp)
target_include_directories(${HELLO_EXEC}
        PUBLIC ThirdParty/matrix
        PRIVATE ThirdParty)
target_link_libraries(${HELLO_EXEC} PUBLIC
        PUBLIC MPI::MPI_CXX)

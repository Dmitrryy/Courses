
cmake_minimum_required(VERSION 3.9)

project(MPI_prog)

set(HELLO_EXEC hello)
add_executable(${HELLO_EXEC} HelloMPI.cpp)

find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
target_link_libraries(${HELLO_EXEC} MPI::MPI_CXX)

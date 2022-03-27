
cmake_minimum_required(VERSION 3.9)

project(MPI_SendREcive)

find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")


set(HELLO_EXEC sr)
add_executable(${HELLO_EXEC} sr.cpp)
target_link_libraries(${HELLO_EXEC} MPI::MPI_CXX)

set(HELLO_EXEC pingpong)
add_executable(${HELLO_EXEC} pingpong.cpp)
target_link_libraries(${HELLO_EXEC} MPI::MPI_CXX)

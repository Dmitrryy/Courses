
cmake_minimum_required(VERSION 3.20)

project(MPI_Exp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")

find_package(Boost REQUIRED)

find_path(MPFR_INCLUDE_DIR mpfr.h)
find_library(MPFR_LIBRARY mpfr)
message(STATUS "gmp: ${GMP_INCLUDE_DIR}, lib: ${GMP_LIBRARY}")
message(STATUS "mpfr: ${MPFR_INCLUDE_DIR}, lib: ${MPFR_LIBRARY}")


set(HELLO_EXEC Exp)
add_executable(${HELLO_EXEC} main.cpp)
target_include_directories(${HELLO_EXEC} PUBLIC
        ${MPFR_INCLUDE_DIR}
        Boost::Headers)
target_link_libraries(${HELLO_EXEC} PUBLIC
        PUBLIC MPI::MPI_CXX
        PUBLIC ${MPFR_LIBRARY}
        Boost::boost)


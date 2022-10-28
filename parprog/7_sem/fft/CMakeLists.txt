cmake_minimum_required(VERSION 3.20)

project(fft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS TRUE)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "-openmp:experimental -openmp:llvm")
# find_package(MPI REQUIRED)
# message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")


set(FFT_EXEC_NAME fft)
add_executable(${FFT_EXEC_NAME} main.cpp fft.cpp)
target_link_libraries(${FFT_EXEC_NAME} PUBLIC
        PUBLIC OpenMP::OpenMP_CXX)

cmake_minimum_required(VERSION 3.20)

project(lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")


set(2B_OMP_EXEX main_2b_omp)
add_executable(${2B_OMP_EXEX} main_2b_omp.cpp)
target_link_libraries(${2B_OMP_EXEX} PUBLIC
        PUBLIC OpenMP::OpenMP_CXX)

set(2B_EXEX main_2b)
add_executable(${2B_EXEX} main_2b.cpp)
target_link_libraries(${2B_EXEX} PUBLIC
        PUBLIC MPI::MPI_CXX)



set(1D_EXEX main_1d)
add_executable(${1D_EXEX} main_1d.cpp)
target_link_libraries(${1D_EXEX} PRIVATE OpenMP::OpenMP_CXX)

set(REF_EXEX main_ref)
add_executable(${REF_EXEX} main_ref.cpp)
target_link_libraries(${REF_EXEX} PRIVATE OpenMP::OpenMP_CXX)
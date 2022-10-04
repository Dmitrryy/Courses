cmake_minimum_required(VERSION 3.10)

# set the project name
project(Matrix)

# specify the C++ standard
#set(MAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_FLAGS "-openmp:experimental -openmp:llvm")

# add the executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC
        PUBLIC OpenMP::OpenMP_CXX)


add_executable(main_fast main_fast.cpp)
target_link_libraries(main_fast PUBLIC
        PUBLIC OpenMP::OpenMP_CXX)
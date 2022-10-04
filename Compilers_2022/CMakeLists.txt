cmake_minimum_required(VERSION 3.10)


project(Compilers_2022)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
#set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

set(GLIB_NAME glib)
add_library(${GLIB_NAME} STATIC glib/SEngine.cpp)
target_link_libraries(${GLIB_NAME} sfml-system sfml-graphics sfml-window)


add_executable(${PROJECT_NAME}
        main.c)
target_link_libraries(${PROJECT_NAME} glib)

#set(LLVM_WITH_GLIB_NAME sdelal)
#add_executable(${LLVM_WITH_GLIB_NAME} 
#        main.ll)
#target_link_libraries(${LLVM_WITH_GLIB_NAME} glib)


find_package(LLVM REQUIRED CONFIG)
add_executable(generator llvmgen.cpp)
target_link_libraries(generator LLVM)
target_include_directories(generator PRIVATE ${LLVM_INCLUDE_DIRS})

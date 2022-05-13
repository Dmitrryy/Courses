#[[
/****************************************************************************************
*
*   CmakeLists.txt
*
*   Created by dmitry
*   9.05.2022
*
***/
]]

cmake_minimum_required(VERSION 3.20)


project(Integral)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


add_executable(integral main.cpp)
add_executable(betterIntegral main2.cpp)

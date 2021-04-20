#[[
/****************************************************************************************
*
*   CmakeLists.txt
*
*   Created by dmitry
*   20.04.2021
*
***/
]]

cmake_minimum_required(VERSION 3.10)


project(Integral1)


find_package(Threads REQUIRED)


add_executable(integral main.c calc.c CPUTopology.c)


target_link_libraries(integral Threads::Threads)
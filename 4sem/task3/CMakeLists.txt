#[[
/****************************************************************************************
*
*   CmakeLists.txt
*
*   Created by dmitry
*   16.05.2021
*
***/
]]

cmake_minimum_required(VERSION 3.10)



project(Integral2)


find_package(Threads REQUIRED)


add_executable(integralNet
        main.c

        src/common.c

        src/Network/worker.c
        src/Network/worker_manager.c

        src/Integration/calc.c
        src/Integration/pthread_manager.c
        )


target_link_libraries(integralNet Threads::Threads)
target_include_directories(integralNet PRIVATE include)
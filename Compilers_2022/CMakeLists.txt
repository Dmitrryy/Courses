cmake_minimum_required(VERSION 3.20)


project(Compilers_2022)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
#set(CMAKE_CXX_EXTENSIONS OFF)

#find_package(Vulkan REQUIRED)
#find_package(glfw3 REQUIRED)
#find_package(glm REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
        SEngine.cpp)

#target_include_directories(${PROJECT_NAME}
#        PUBLIC Vulkan::Headers)
#target_link_libraries(${PROJECT_NAME}
        #Vulkan::Vulkan
        #glfw
        #glm::glm
#        )
target_link_libraries(${PROJECT_NAME} sfml-system sfml-graphics sfml-window)

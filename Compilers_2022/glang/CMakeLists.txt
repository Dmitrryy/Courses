cmake_minimum_required(VERSION 3.10)


project(glang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(LLVM REQUIRED)


flex_target(
        scanner
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/lex.l
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/lexer.cpp
        )

bison_target(
        grammar
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/gramar.y
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.cpp
        COMPILE_FLAGS "--defines=${CMAKE_CURRENT_SOURCE_DIR}/parser/compiler.cpp.h"
)

add_flex_bison_dependency(scanner grammar)

add_executable(${PROJECT_NAME} 
${BISON_grammar_OUTPUTS}
${FLEX_scanner_OUTPUTS}
Compiler/builtins.cpp
Compiler/utils.cpp
driver.cpp
)
target_link_libraries(${PROJECT_NAME} LLVM)
target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS})


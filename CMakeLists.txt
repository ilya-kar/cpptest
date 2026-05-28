cmake_minimum_required(VERSION 3.16)
project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/bst)

add_executable(test src/main.cpp)

target_link_libraries(test PRIVATE
        bst
)

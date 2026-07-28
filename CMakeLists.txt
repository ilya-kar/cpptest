cmake_minimum_required(VERSION 3.16)
project(avl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTING "Build tests" ON)

add_library(avl INTERFACE)

target_include_directories(avl INTERFACE ${PROJECT_SOURCE_DIR}/include)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()


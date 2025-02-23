cmake_minimum_required(VERSION 3.20)

project(MP4 CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

add_compile_options(-g)
# 调试时开
add_compile_options(-O1)

include_directories(include)

add_subdirectory(src src)
add_subdirectory(test test)

cmake_minimum_required(VERSION 3.7)
project(Lab5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(SOURCE_FILES gpio_blink.c)
add_executable(Lab5 ${SOURCE_FILES})
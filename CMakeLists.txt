cmake_minimum_required(VERSION 3.9)
project(testingThreading)
set(CMAKE_CXX_FLAGS "-lncurses")

set(CMAKE_CXX_STANDARD 11)
add_executable(testingThreading main.cpp)
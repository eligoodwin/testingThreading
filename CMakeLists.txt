cmake_minimum_required(VERSION 3.8.2)
project(testingThreading)
set(CMAKE_CXX_FLAGS "-lncurses -pthread")

set(CMAKE_CXX_STANDARD 11)
add_executable(testingThreading main.cpp RingBuffer.cpp RingBuffer.h)
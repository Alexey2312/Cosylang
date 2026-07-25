cmake_minimum_required(VERSION 3.28)
project(Cosylang CXX)


set(PROJECT_SOURCES
    src/main.cpp
    tests/utills-tests/arena-allocator/arena-alloc-test.cpp
)

add_executable(Cosylang ${PROJECT_SOURCES})

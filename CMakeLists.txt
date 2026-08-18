cmake_minimum_required(VERSION 3.28)
project(Cosylang CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
else()
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ot")
endif()

set(PROJECT_SOURCES
    src/main.cpp
    src/frontend/lexer/lexer.cpp
    tests/utills-tests/arena-allocator/arena-alloc-test.cpp
    tests/lexer-tests/lexer-tests.cpp
)

add_executable(Cosylang ${PROJECT_SOURCES})

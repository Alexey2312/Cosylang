cmake_minimum_required(VERSION 3.28)
project(Cosylang CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    src/main.cpp
    src/frontend/lexer/lexer.cpp
    src/frontend/parser/parser.cpp

    tests/utills-tests/arena-allocator/arena-alloc-test.cpp
    tests/lexer-tests/lexer-tests.cpp
    tests/parser-tests/parse-expression-tests.cpp
    tests/parser-tests/parse-keyword-tests.cpp

)

add_executable(Cosylang ${PROJECT_SOURCES})

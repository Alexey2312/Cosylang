cmake_minimum_required(VERSION 3.28)
project(Cosylang CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    src/main.cpp
    src/frontend/lexer/lexer.cpp
    src/frontend/parser/parser.cpp
    src/diagnostics/error-reporter.cpp
    src/frontend/semantic/scope.cpp

    tests/diagnostics-tests/reporter-tests.cpp
    tests/utills-tests/arena-allocator/arena-alloc-test.cpp
    tests/lexer-tests/lexer-tests.cpp
    tests/parser-tests/parse-expression-tests.cpp
    tests/parser-tests/parse-keyword-tests.cpp
    tests/semantic-tests/scope-tests.cpp

)

add_executable(Cosylang ${PROJECT_SOURCES})

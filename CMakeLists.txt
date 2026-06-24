cmake_minimum_required(VERSION 3.28)
project(Cosylang CXX)


set(PROJECT_SOURCES
    src/main.cpp
    src/frontend/lexer/token/token.cpp
)

add_executable(Cosylang ${PROJECT_SOURCES})

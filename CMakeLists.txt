cmake_minimum_required(VERSION 3.31.6)

project(Cosylang)

set(SUB_DIRS
    "src/frontend/code/parser"
)

foreach(DIR ${SUB_DIRS})
    file(GLOB FILES CONFIGURE_DEPENDS "${DIR}/*.cpp")
    list(APPEND ALL_SOURCES ${FILES})
endforeach()

add_executable(Cosylang ${ALL_SOURCES})

target_include_directories(Cosylang PRIVATE ${SUB_DIRS})

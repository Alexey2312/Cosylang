cmake_minimum_required(VERSION 3.31.6)

project(Cosylang)

find_package(LLVM 17 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND "${LLVM_DEFINITIONS}")
add_compile_options(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs core support native analysis executionengine)
target_link_libraries(my_compiler PRIVATE ${llvm_libs})

set(SUB_DIRS
    "src/frontend/code/parser"
)

foreach(DIR ${SUB_DIRS})
    file(GLOB FILES CONFIGURE_DEPENDS "${DIR}/*.cpp")
    list(APPEND ALL_SOURCES ${FILES})
endforeach()

add_executable(Cosylang ${ALL_SOURCES})

target_include_directories(Cosylang PRIVATE ${SUB_DIRS})

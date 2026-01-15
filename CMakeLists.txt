cmake_minimum_required(VERSION 3.31)

project(Cosylang)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake from ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs core support native analysis executionengine)

set(SUB_DIRS
    "src/frontend/code/semantic"
)

set(ALL_SOURCES "")
foreach(DIR ${SUB_DIRS})
    file(GLOB FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/${DIR}/*.cpp")
    list(APPEND ALL_SOURCES ${FILES})
endforeach()

add_executable(Cosylang ${ALL_SOURCES})

target_include_directories(Cosylang PRIVATE ${SUB_DIRS} ${LLVM_INCLUDE_DIRS})
target_link_libraries(Cosylang PRIVATE ${llvm_libs})

target_compile_features(Cosylang PRIVATE cxx_std_23)

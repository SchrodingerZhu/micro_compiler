cmake_minimum_required(VERSION 3.19)
project(micro_compiler)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(frontend)
add_subdirectory(backend)
include_directories(frontend/include)
include_directories(backend/include)
include_directories(include)
add_executable(mcompiler main.cpp builtin.cpp codegen.cpp)

target_link_libraries(mcompiler parser vcfg)

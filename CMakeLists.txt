cmake_minimum_required(VERSION 3.19)
project(micro_compiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -g -flto -pie")
add_subdirectory(frontend)
add_subdirectory(backend)
include_directories(backend/parallel-hashmap/parallel_hashmap)
include_directories(frontend/include)
include_directories(backend/include)
include_directories(include)
add_executable(mcompiler main.cpp builtin.cpp codegen.cpp)

target_link_libraries(mcompiler parser vcfg)

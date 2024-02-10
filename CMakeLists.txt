cmake_minimum_required(VERSION 3.10)
project(Sinus)

set(CMAKE_CXX_STANDARD 11)

add_executable(program_float 1.cpp)
target_compile_definitions(program_float PRIVATE ARRAY_TYPE=float)

add_executable(program_double 1.cpp)
target_compile_definitions(program_double PRIVATE ARRAY_TYPE=double)

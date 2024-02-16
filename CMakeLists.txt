cmake_minimum_required(VERSION 3.10)

project(Sinus)

add_executable(1 1.cpp)

if(BUILD_TYPE STREQUAL "float")
    add_compile_definitions(FLOAT)
elseif(BUILD_TYPE STREQUAL "double")
    add_compile_definitions(DOUBLE)
endif()

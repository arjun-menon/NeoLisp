cmake_minimum_required(VERSION 3.5)
project(VectorCalc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES main.cpp stdinclude.h data_reader.cpp data_reader.h)
add_executable(vcalc ${SOURCE_FILES})

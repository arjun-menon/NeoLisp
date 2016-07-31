cmake_minimum_required(VERSION 3.5)
project(VectorCalculator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES src/main.cpp src/stdinclude.h src/data_reader.cpp src/data_reader.h src/table.cpp src/table.h src/column.cpp src/column.h src/cell.cpp src/cell.h src/utils.h)

add_executable(vcalc ${SOURCE_FILES})

cmake_minimum_required(VERSION 3.5)
project(VectorCalculator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES
        src/cell.h
        src/cell.cpp
        src/column.h
        src/column.cpp
        src/table.h
        src/table.cpp
        src/data_reader.h
        src/data_reader.cpp
        src/stdinclude.h
        src/utils.h
)

add_executable(vcalc ${SOURCE_FILES} src/main.cpp)

add_executable(vcalc_unit_tests ${SOURCE_FILES} tests/catch.hpp tests/unit_tests.cpp)
enable_testing()
add_test(NAME unit_tests COMMAND vcalc_unit_tests)

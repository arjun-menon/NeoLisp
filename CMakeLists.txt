cmake_minimum_required(VERSION 3.5)
project(VectorCalculator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")

set(SOURCE_FILES
        src/cell.h
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

set(UNIT_TEST_FILES
        unit_tests/catch.hpp
        unit_tests/catch_main.cpp
        unit_tests/cell_tests.cpp
)

add_executable(vcalc_unit_tests ${SOURCE_FILES} ${UNIT_TEST_FILES})
enable_testing()
add_test(NAME unit_tests COMMAND vcalc_unit_tests)

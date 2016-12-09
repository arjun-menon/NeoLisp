cmake_minimum_required(VERSION 3.5)
include(CheckIncludeFileCXX)
project(vcalc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wall -g")

set(SOURCE_FILES
        src/lexer.cpp src/lexer.h
        src/parser.cpp src/parser.h
        src/datatypes.h src/datatypes.cpp
        src/column.h src/column.cpp
        src/table.h src/table.cpp
        src/data_reader.h src/data_reader.cpp
        src/utils.h src/common.h)

add_executable(vcalc ${SOURCE_FILES} src/main.cpp)

set_property(TARGET vcalc PROPERTY CXX_STANDARD 11)
set_property(TARGET vcalc PROPERTY CXX_STANDARD_REQUIRED ON)

set(UNIT_TEST_FILES
        unit_tests/catch_main.cpp
        unit_tests/datatypes_tests.cpp
)

check_include_file_cxx(catch.hpp HAVE_CATCH)

if(HAVE_CATCH)
    add_executable(vcalc_unit_tests ${SOURCE_FILES} ${UNIT_TEST_FILES})
    enable_testing()
    add_test(NAME unit_tests COMMAND vcalc_unit_tests)
else(Y)
    message("If you'd like to run unit tests, install Catch: https://github.com/philsquared/Catch")
endif()

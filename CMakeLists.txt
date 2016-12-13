cmake_minimum_required(VERSION 3.5)
include(CheckIncludeFileCXX)
project(vcalc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wall -Werror -g")

set(SOURCE_FILES
        src/lexer.cpp src/lexer.hpp
        src/parser.cpp src/parser.hpp
        src/datatypes.hpp src/datatypes.cpp
        src/column.hpp src/column.cpp
        src/table.hpp src/table.cpp
        src/data_reader.hpp src/data_reader.cpp
        src/common.hpp)

add_executable(vcalc ${SOURCE_FILES} src/main.cpp)

set_property(TARGET vcalc PROPERTY CXX_STANDARD 11)
set_property(TARGET vcalc PROPERTY CXX_STANDARD_REQUIRED ON)

set(UNIT_TEST_FILES
        unit_tests/catch_main.cpp
        unit_tests/datatypes_tests.cpp
        unit_tests/lexer_tests.cpp
)

check_include_file_cxx(catch.hpp HAVE_CATCH)

if(HAVE_CATCH)
    add_executable(vcalc_unit_tests ${SOURCE_FILES} ${UNIT_TEST_FILES})
    enable_testing()
    add_test(NAME unit_tests COMMAND vcalc_unit_tests)
else(Y)
    message("If you'd like to run unit tests, install Catch: https://github.com/philsquared/Catch")
endif()

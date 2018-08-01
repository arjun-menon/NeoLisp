cmake_minimum_required(VERSION 3.5)
include(CheckIncludeFileCXX)
project(NeoLisp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Werror -g")

set(SOURCE_FILES
        src/lexer.cpp src/lexer.hpp
        src/parser.cpp src/parser.hpp
        src/datatypes.hpp src/datatypes.cpp
        src/eval.cpp src/eval.h
        src/builtin.cpp
        src/matrix.hpp src/matrix.cpp
        src/utils.hpp src/common.hpp)

add_executable(NeoLisp ${SOURCE_FILES} src/main.cpp)

set_property(TARGET NeoLisp PROPERTY CXX_STANDARD 17)
set_property(TARGET NeoLisp PROPERTY CXX_STANDARD_REQUIRED ON)

set(UNIT_TEST_FILES
        unit_tests/catch_main.cpp
        unit_tests/datatypes_tests.cpp
        unit_tests/lexer_tests.cpp
        unit_tests/language_tests.cpp)

check_include_file_cxx(catch.hpp HAVE_CATCH)

if(HAVE_CATCH)
    add_executable(NeoLisp-tests ${SOURCE_FILES} ${UNIT_TEST_FILES})
    enable_testing()
    add_test(NAME NeoLisp-tests COMMAND NeoLisp-tests)
else(Y)
    message("If you'd like to run unit tests, install Catch: https://github.com/philsquared/Catch")
endif()

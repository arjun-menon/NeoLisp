#!/usr/bin/env bash

# Run cpplint, https://pypi.python.org/pypi/cpplint
#cpplint "$@" src/*

# Run cppcheck
cppcheck -j 8 --language=c++ --std=c++14 --enable=warning,performance,portability,unusedFunction --platform=unix64 src

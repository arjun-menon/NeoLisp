#!/usr/bin/env bash

# Run cpplint, https://pypi.python.org/pypi/cpplint
#cpplint "$@" src/*

# Run cppcheck (add `-j 8` for parallelism; this disables the unusedFunction check however)
cppcheck -j 8 --language=c++ --std=c++14 --enable=warning,performance,portability,unusedFunction --platform=unix64 --error-exitcode=5 src
exit $?

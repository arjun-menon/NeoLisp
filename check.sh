# C++ Code Style Check

# Run cpplint, https://pypi.python.org/pypi/cpplint
cpplint "$@" src/*

# Run cppcheck
#cppcheck -j 8 --language=c++ --std=c++11 --enable=warning,performance,unusedFunction,style src

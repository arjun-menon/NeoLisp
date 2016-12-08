# Code Quality Checks

# Run cppcheck
cppcheck -j 8 --language=c++ --std=c++11 --enable=warning,performance,unusedFunction,style src

# Run valgrind
valgrind --leak-check=full --track-origins=yes --suppressions=tools/.mac_leak.supp ./cbuild/vcalc_unit_tests

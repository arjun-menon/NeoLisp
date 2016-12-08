# Run valgrind
valgrind --leak-check=full --track-origins=yes --suppressions=tools/.mac_leak.supp ./cbuild/vcalc_unit_tests

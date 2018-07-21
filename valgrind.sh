#!/usr/bin/env bash

# Run valgrind
valgrind --leak-check=full --track-origins=yes --suppressions=tools/.mac_leak.supp ./cbuild/NeoLisp-tests

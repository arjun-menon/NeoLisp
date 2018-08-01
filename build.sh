#!/usr/bin/env bash

set -e

# Should clean?
if [[ $* == *--clean* ]]; then
  rm -rf cbuild/
  rm -f NeoLisp
  rm -f NeoLisp-tests
  rm -rf *.dSYM/
  exit 0
fi

if ! [ -x "$(command -v cppcheck)" ]; then
    printf "Install cppcheck to perform additional code quality checks.\n\n"
  else
    printf "Running `cppcheck --version`...\n"
    # Run cppcheck (`-j 8` for parallelism disables the unusedFunction check ; cppcheck does not support C++17 yet, so we set --std=c++14)
    cppcheck -j 8 --language=c++ --std=c++14 --enable=warning,performance,portability,unusedFunction --platform=unix64 --error-exitcode=5 src || true
    printf "\n"
fi

# Create build directory
mkdir -p cbuild
cd cbuild

# Generate Makefile
printf "Running cmake...\n"
cmake ..

# Get the number of CPUs
printf "\nNumber of CPUs: " -n
NUMCPUS=4
OS=`uname`
if [[ "$OS" == 'Linux' ]]; then
  NUMCPUS=`grep -c '^processor' /proc/cpuinfo`
elif [[ "$OS" == 'Darwin' ]]; then
  NUMCPUS=`sysctl -n hw.ncpu`
fi
printf "%s\n" "$NUMCPUS"

# Run make
echo "Compiling..."
make -j$NUMCPUS

cd ..

# Symlink from base dir
echo "Done compiling."
ln -sf cbuild/NeoLisp
ln -sf cbuild/NeoLisp-tests

TESTS=./cbuild/NeoLisp-tests
if [ -f $TESTS ]; then
  # Run unit tests
  printf "\nRunning unit tests...\n\n"
  if ! [ -x "$(command -v valgrind)" ]; then
    printf "Install Valgrind to perform memory leak checks.\n\n"
    $TESTS
  else
    # Run valgrind
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=tools/.mac_leak.supp $TESTS
  fi
fi

# Done
if [ $? -eq 0 ]; then
  printf "\nYou can run ./NeoLisp now.\n"
  exit 0
else
  exit $?
fi

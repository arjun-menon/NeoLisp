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

if [[ $* != *--notest* ]]; then
  if ! [ -x "$(command -v cppcheck)" ]; then
      printf "Install cppcheck to perform additional code quality checks.\n\n"
    else
      printf "Running `cppcheck --version`...\n"
      # Run cppcheck:
      # Note: Cppcheck sometimes generates false positives and does not support C++17 yet, so we ignore its exit code.
      #       We use it just to bring attention to potential bugs. Also, you can add `unusedFunction` to the list of
      #       checks enabled below, if the `-j 8` parallelism flag has been removed (as `j -8` disables the  check).
      cppcheck -j 8 --language=c++ --enable=warning,performance,portability --platform=unix64 --error-exitcode=5 src || true
      printf "\n"
  fi
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
if [[ $* != *--notest* ]]; then
  make -j$NUMCPUS
else
  make -j$NUMCPUS NeoLisp
fi

cd ..

# Symlink from base dir
echo "Done compiling."
ln -sf cbuild/NeoLisp
if [[ $* != *--notest* ]]; then
  ln -sf cbuild/NeoLisp-tests
fi

# Run unit tests
if [[ $* != *--notest* ]]; then
  TESTS=./cbuild/NeoLisp-tests
  if [ -f $TESTS ]; then
    printf "\nRunning unit tests...\n\n"
    if ! [ -x "$(command -v valgrind)" ]; then
      printf "Install Valgrind to perform memory leak checks.\n\n"
      $TESTS
    else
      # Run valgrind
      valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=.valgrind-false-positives.supp --gen-suppressions=all --error-exitcode=999 $TESTS
    fi
  fi
fi

# Done
if [ $? -eq 0 ]; then
  printf "\nYou can run ./NeoLisp now.\n"
  exit 0
else
  exit $?
fi

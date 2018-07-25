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

if [ -f cbuild/NeoLisp-tests ]; then
  # Run unit tests
  printf "\nRunning unit tests...\n\n"
  if ! [ -x "$(command -v valgrind)" ]; then
    printf "Install Valgrind to perform memory leak checks.\n\n"
    ./cbuild/NeoLisp-tests
  else
    ./valgrind.sh ./cbuild/NeoLisp-tests
  fi
fi

# Done
if [ $? -eq 0 ]; then
  printf "\nYou can run ./NeoLisp now.\n"
  exit 0
else
  exit $?
fi

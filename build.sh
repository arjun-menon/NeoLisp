#!/usr/bin/env bash

set -e

# Should clean?
if [[ $* == *--clean* ]]; then
  rm -rf cbuild/
  rm -f vcalc
  rm -f vcalc_unit_tests
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
ln -sf cbuild/vcalc

if [ -f cbuild/vcalc_unit_tests ]; then
  # Run unit tests
  printf "\nRunning unit tests...\n"
  ./cbuild/vcalc_unit_tests
fi

# Done
echo "Feel free to run ./vcalc"

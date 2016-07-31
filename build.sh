#!/usr/bin/env bash

set -e

mkdir -p build_files
cd build_files

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
echo "Setting up symlinks..."
ln -sf build_files/vcalc
ln -sf build_files/vcalc_unit_tests

# Run unit tests
printf "\nRunning unit tests...\n"
./vcalc_unit_tests

# Done
echo "Feel free to run ./vcalc"

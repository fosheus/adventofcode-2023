#!/bin/bash

# Check if both arguments are provided
if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <x> <y> [input file]"
  exit 1
fi

# Extract values for x and y from arguments
x=$1
y=$2

# Construct the binary path
binary="bin/${x}_${y}"

# Check if the binary exists
if [ ! -x "$binary" ]; then
  echo "Error: Binary '$binary' not found or not executable."
  exit 1
fi

# Check if a third argument is provided
if [ "$#" -eq 3 ]; then
  arg3=$3
  argument="docs/$x/$arg3"
else
  argument="docs/$x/$x.txt"
fi

# Execute the binary with the constructed argument
$binary "$argument"
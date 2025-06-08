#!/usr/bin/env bash

set -e

# Ensure script is run with sudo

working_dir=$(pwd)
build_dir="$working_dir/build"
mount_point="/home/travmonkey/rpi-pico"

echo "Attempting to build the project"

# Check for --clean or -c flag
if [[ "$@" =~ (--clean|-c) ]]; then
  echo "Cleaning build directory..."
  rm -rf "$build_dir"
fi

mkdir -p "$build_dir"
cd "$build_dir" || {
  echo "Failed to access build directory"
  exit 1
}

# Run CMake and Make
if cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && make; then
  echo "Project built successfully"
else
  echo "Build failed"
  exit 1
fi

# Find RP2040 device
echo "Searching for a connected Pico"
device=$(lsblk -o NAME,MODEL | grep "RP2" | awk '{print $1}')

if [ -z "$device" ]; then
  echo "Device RP2 not found"
  exit 1
fi

echo "Pico found: $device"

mount_path="/dev/${device}1"
echo "Mounting $mount_path to $mount_point"
mkdir -p "$mount_point"
mount "$mount_path" "$mount_point"

# Determine whether to flash master or slave
if [[ "$@" =~ (--slave) ]]; then
  echo "Flashing slave to connected Pico"
  cp ./src/devices/left_half/left_half.uf2 "$mount_point/"
else
  echo "Flashing master to connected Pico"
  cp ./src/devices/right_half/right_half.uf2 "$mount_point/"
fi

echo "Completed mounting and flashing Pico"

#!/usr/bin/env bash

working_dir=$(pwd)
echo "Attempting to build the project"
cd "$working_dir/build" || {
  echo "No build directory found ... exiting"
  exit
}

if cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..; then
  if make; then
    echo "Project built successfully"
  else
    echo "Make failed, check the configuration and logs"
    exit
  fi
else
  echo "Cmake failed, check the configuration and logs"
  exit
fi

# Find the device with the specified disk name
echo "Searching for a connected Pico"
device=$(lsblk -o NAME,MODEL | grep "RP2" | awk '{print $1}')

# Exit if no RPico is found
if [ -z "$device" ]; then
  echo "Device RP2 not found"
  exit 1
fi

echo "Pico found:"
sudo mount /dev/"$device"1 /home/travis/rpi-pico
echo "Device $device mounted at /home/travis/rpi-pico/"

echo "Flashing Master by default, add --slave to flash Slave"
if [ -z "$1" ]; then
  echo 'Flashing sender to connected Pico'
  sudo cp ./src/core/sender.uf2 /home/travis/rpi-pico/
elif [ "$1" = "--slave" ]; then
  echo "Flashing Slave to connected Pico"
  sudo cp ./src/core/receiver.uf2 /home/travis/rpi-pico/
else 
  echo "Invalid argument. Use --slave to flash Slave firmware."
  exit 1
fi

echo "Completed mounting and flashing Pico"

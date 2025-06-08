cmake_minimum_required(VERSION 3.13)

# Set the pico_sdk_path variable to the path of the pico-sdk
set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/pico-sdk)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)
project(travyboardv2 C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
  message(
    FATAL_ERROR
      "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}. Please update the submodule."
  )
endif()

set(FAMILY rp2040)
set(BOARD pico_sdk)
set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "travyboard_dev_")

# Initialize the SDK
pico_sdk_init()

include(./lib/RF24/CMakeLists.txt)

add_compile_options(
  -Wall -Wno-format # int != int32_t as far as the compiler is concerned because
                    # gcc has int32_t as long int
  -Wno-unused-function # we have some for the docs that aren't called
)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wno-maybe-uninitialized)
endif()

# Keyboard src
add_subdirectory(src)

add_executable(right_half)
target_include_directories(
  right_half PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../../core
                ${CMAKE_CURRENT_LIST_DIR}/../../tusb
                ${CMAKE_CURRENT_LIST_DIR}/../../keymaps
                ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/keymap_files)
target_sources(right_half PUBLIC right_half.c 
  ${CMAKE_CURRENT_LIST_DIR}/../../core/keyboard.c
  ${CMAKE_CURRENT_LIST_DIR}/../../core/matrix.c
  ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/keymap.c
  ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/layer_processor.c
  ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/keymap_files/default.c
  ${CMAKE_CURRENT_LIST_DIR}/../../tusb/usb_descriptors.c)
target_link_libraries(
  right_half
  PUBLIC pico_stdlib
         tinyusb_device
         tinyusb_board
         hardware_i2c
         hardware_spi
         hardware_uart
         RF24)
pico_enable_stdio_usb(right_half 0)
pico_enable_stdio_uart(right_half 1)
pico_add_extra_outputs(right_half)

add_executable(left_half)
target_include_directories(
  left_half PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../../core
                ${CMAKE_CURRENT_LIST_DIR}/../../core/matrix
                ${CMAKE_CURRENT_LIST_DIR}/../../tusb
                ${CMAKE_CURRENT_LIST_DIR}/../../keymaps
                ${CMAKE_CURRENT_LIST_DIR}/../../transport/uart
                ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/keymap_files)
target_sources(left_half PUBLIC left_half.c 
  ${CMAKE_CURRENT_LIST_DIR}/../../core/keyboard.c
  ${CMAKE_CURRENT_LIST_DIR}/../../core/debounce.c
  ${CMAKE_CURRENT_LIST_DIR}/../../core/matrix/matrix.c
  ${CMAKE_CURRENT_LIST_DIR}/../../core/matrix/matrix_single.c
  ${CMAKE_CURRENT_LIST_DIR}/../../core/matrix/matrix_split.c
  ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/keymap.c
  ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/keymap_single.c
  ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/keymap_split.c
  ${CMAKE_CURRENT_LIST_DIR}/../../transport/uart/uart.c
  ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/layer_processor.c
  ${CMAKE_CURRENT_LIST_DIR}/../../keymaps/keymap_files/default.c
  ${CMAKE_CURRENT_LIST_DIR}/../../tusb/usb_descriptors.c)

pico_enable_stdio_uart(left_half 1)

target_link_libraries(
  left_half
  PUBLIC pico_stdlib
         tinyusb_device
         tinyusb_board
         hardware_i2c
         hardware_spi
         hardware_uart
         RF24)
pico_add_extra_outputs(left_half)

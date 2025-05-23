# add_executable(sender)
# target_include_directories(
#   sender PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../tusb
#                 ${CMAKE_CURRENT_LIST_DIR}/../transport
#                 ${CMAKE_CURRENT_LIST_DIR}/../keymaps
#                 ${CMAKE_CURRENT_LIST_DIR}/../keymaps/keymap_files)
# target_sources(sender PUBLIC sender.c 
#   ${CMAKE_CURRENT_LIST_DIR}/../transport/usb_descriptors.c
#   ${CMAKE_CURRENT_LIST_DIR}/../keymaps/keymap.c
#   ${CMAKE_CURRENT_LIST_DIR}/../keymaps/layer_processor.c
#   ${CMAKE_CURRENT_LIST_DIR}/../keymaps/keymap_files/default.c)
# target_link_libraries(
#   sender
#   PUBLIC pico_stdlib
#          tinyusb_device
#          tinyusb_board
#          hardware_i2c
#          hardware_spi
#          hardware_uart
#          RF24)
# pico_add_extra_outputs(sender)
#
# add_executable(receiver)
# target_include_directories(
#   receiver PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../tusb
#                 ${CMAKE_CURRENT_LIST_DIR}/../transport
#                 ${CMAKE_CURRENT_LIST_DIR}/../keymaps
#                 ${CMAKE_CURRENT_LIST_DIR}/../keymaps/keymap_files)
# target_sources(receiver PUBLIC receiver.c
#   ${CMAKE_CURRENT_LIST_DIR}/../transport/usb_descriptors.c
#   ${CMAKE_CURRENT_LIST_DIR}/../keymaps/keymap.c
#   ${CMAKE_CURRENT_LIST_DIR}/../keymaps/layer_processor.c
#   ${CMAKE_CURRENT_LIST_DIR}/../keymaps/keymap_files/default.c)
# target_link_libraries(
#   receiver
#   PUBLIC pico_stdlib
#          tinyusb_device
#          tinyusb_board
#          hardware_spi
#          hardware_i2c
#          hardware_uart
#          RF24)
# pico_add_extra_outputs(receiver)

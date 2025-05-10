add_executable(sender sender.cpp)
target_include_directories(sender PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../config
    ${CMAKE_CURRENT_LIST_DIR}/../transport
)
target_link_libraries(sender PUBLIC pico_stdlib tinyusb_device tinyusb_board hardware_i2c hardware_spi hardware_uart RF24)
pico_add_extra_outputs(sender)

add_executable(receiver receiver.cpp)
target_include_directories(sender PRIVATE ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../config
    ${CMAKE_CURRENT_LIST_DIR}/../transport
)
target_link_libraries(receiver PUBLIC pico_stdlib tinyusb_device tinyusb_board hardware_spi hardware_i2c hardware_uart RF24)
pico_add_extra_outputs(receiver)

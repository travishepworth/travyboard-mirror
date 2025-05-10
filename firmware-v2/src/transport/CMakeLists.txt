add_library(transport_usb STATIC
  usb_descriptors.c
  usb_descriptors.h
)

target_include_directories(transport_usb PUBLIC ${CMAKE_CURRENT_LIST_DIR})

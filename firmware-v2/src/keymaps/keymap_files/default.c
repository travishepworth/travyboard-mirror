#include "default.h"
#include "class/hid/hid.h"
#include "layer_processor.h"

// TODO: Make this readable
const uint8_t keymap_default[TOTAL_LAYERS][TOTAL_ROWS][TOTAL_COLS] __attribute__((section(".rodata"))) = {
// const uint8_t keymap_default[TOTAL_LAYERS][TOTAL_ROWS][TOTAL_COLS] = {
  {
    {'\0', HID_KEY_Y, HID_KEY_U, HID_KEY_I, HID_KEY_O, HID_KEY_P, HID_KEY_BACKSPACE},
    {'\0', HID_KEY_H, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_SEMICOLON, HID_KEY_ENTER},
    {'\0', HID_KEY_N, HID_KEY_M, HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH, HID_KEY_SHIFT_RIGHT},
    {'\0', HID_KEY_SPACE, LAYER_KEY_M01, '\0','\0','\0','\0'},
  },
  {
    {'\0', HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_0, HID_KEY_MINUS},
    {'\0', HID_KEY_ARROW_LEFT, HID_KEY_ARROW_DOWN, HID_KEY_ARROW_UP, HID_KEY_ARROW_RIGHT, HID_KEY_APOSTROPHE, '\0'},
    {'\0', '\0', '\0', '\0', '\0', '\0','\0'},
    {'\0', '\0', '\0', '\0','\0','\0','\0'},
  },
  {
    {'\0', HID_KEY_6, HID_KEY_7, HID_KEY_BACKSLASH, HID_KEY_BRACKET_LEFT, HID_KEY_BRACKET_RIGHT, HID_KEY_EQUAL},
    {'\0', HID_KEY_HOME, HID_KEY_INSERT, HID_KEY_END, HID_KEY_DELETE, '\0', HID_KEY_F12},
    {'\0', '\0', '\0', '\0', '\0', '\0','\0'},
    {'\0', '\0', '\0', '\0','\0','\0','\0'},
  },
  {
    {'\0', HID_KEY_Y, HID_KEY_U, HID_KEY_I, HID_KEY_O, HID_KEY_P, HID_KEY_BACKSPACE},
    {'\0', HID_KEY_H, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_SEMICOLON, HID_KEY_ENTER},
    {'\0', HID_KEY_N, HID_KEY_M, HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH, '\0'},
    {'\0', HID_KEY_SPACE, 2, '\0','\0','\0','\0'},
  }
};


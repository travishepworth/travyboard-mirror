#pragma once

#include "class/hid/hid.h"

// Enum definition for all keycodes used in the firmware
typedef enum {
  // Unassigned / Transparent
  KC_NO = 0,

  // Alphabet
  KC_A = HID_KEY_A,
  KC_B = HID_KEY_B,
  KC_C = HID_KEY_C,
  KC_D = HID_KEY_D,
  KC_E = HID_KEY_E,
  KC_F = HID_KEY_F,
  KC_G = HID_KEY_G,
  KC_H = HID_KEY_H,
  KC_I = HID_KEY_I,
  KC_J = HID_KEY_J,
  KC_K = HID_KEY_K,
  KC_L = HID_KEY_L,
  KC_M = HID_KEY_M,
  KC_N = HID_KEY_N,
  KC_O = HID_KEY_O,
  KC_P = HID_KEY_P,
  KC_Q = HID_KEY_Q,
  KC_R = HID_KEY_R,
  KC_S = HID_KEY_S,
  KC_T = HID_KEY_T,
  KC_U = HID_KEY_U,
  KC_V = HID_KEY_V,
  KC_W = HID_KEY_W,
  KC_X = HID_KEY_X,
  KC_Y = HID_KEY_Y,
  KC_Z = HID_KEY_Z,

  // Numbers
  KC_1 = HID_KEY_1,
  KC_2 = HID_KEY_2,
  KC_3 = HID_KEY_3,
  KC_4 = HID_KEY_4,
  KC_5 = HID_KEY_5,
  KC_6 = HID_KEY_6,
  KC_7 = HID_KEY_7,
  KC_8 = HID_KEY_8,
  KC_9 = HID_KEY_9,
  KC_0 = HID_KEY_0,

  // Symbols & Punctuation
  KC_TAB    = HID_KEY_TAB,
  KC_ESC    = HID_KEY_ESCAPE,
  KC_GRV    = HID_KEY_GRAVE,
  KC_BSPC   = HID_KEY_BACKSPACE,
  KC_ENT    = HID_KEY_ENTER,
  KC_SPC    = HID_KEY_SPACE,
  KC_MINS   = HID_KEY_MINUS,
  KC_EQL    = HID_KEY_EQUAL,
  KC_LBRC   = HID_KEY_BRACKET_LEFT,
  KC_RBRC   = HID_KEY_BRACKET_RIGHT,
  KC_BSLS   = HID_KEY_BACKSLASH,
  KC_SCLN   = HID_KEY_SEMICOLON,
  KC_QUOT   = HID_KEY_APOSTROPHE,
  KC_COMM   = HID_KEY_COMMA,
  KC_DOT    = HID_KEY_PERIOD,
  KC_SLSH   = HID_KEY_SLASH,

  // Modifiers
  KC_LGUI   = HID_KEY_GUI_LEFT,
  KC_RGUI   = HID_KEY_GUI_RIGHT,
  KC_LCTL   = HID_KEY_CONTROL_LEFT,
  KC_RCTL   = HID_KEY_CONTROL_RIGHT,
  KC_LSFT   = HID_KEY_SHIFT_LEFT,
  KC_RSFT   = HID_KEY_SHIFT_RIGHT,
  KC_LALT   = HID_KEY_ALT_LEFT,
  KC_RALT   = HID_KEY_ALT_RIGHT,

  // Function Keys
  KC_F1     = HID_KEY_F1,
  KC_F2     = HID_KEY_F2,
  KC_F3     = HID_KEY_F3,
  KC_F4     = HID_KEY_F4,
  KC_F5     = HID_KEY_F5,
  KC_F6     = HID_KEY_F6,
  KC_F7     = HID_KEY_F7,
  KC_F8     = HID_KEY_F8,
  KC_F9     = HID_KEY_F9,
  KC_F10    = HID_KEY_F10,
  KC_F11    = HID_KEY_F11,
  KC_F12    = HID_KEY_F12,

  // Navigation
  KC_LEFT   = HID_KEY_ARROW_LEFT,
  KC_DOWN   = HID_KEY_ARROW_DOWN,
  KC_UP     = HID_KEY_ARROW_UP,
  KC_RIGHT  = HID_KEY_ARROW_RIGHT,
  KC_HOME   = HID_KEY_HOME,
  KC_END    = HID_KEY_END,
  KC_INS    = HID_KEY_INSERT,
  KC_DEL    = HID_KEY_DELETE,

  // Consumer / Media Keys
  KC_BRID   = HID_USAGE_CONSUMER_BRIGHTNESS_DECREMENT,
  KC_BRIP   = HID_USAGE_CONSUMER_BRIGHTNESS_INCREMENT,
  KC_MPRV   = HID_USAGE_CONSUMER_SCAN_PREVIOUS,
  KC_MNXT   = HID_USAGE_CONSUMER_SCAN_NEXT,
  KC_MPLY   = HID_USAGE_CONSUMER_PLAY_PAUSE,

  // Custom Layer Keys
  KC_M01   = 0xFF, // Switch to Layer 1
  KC_M02   = 0xF0,             // Switch to Layer 2
  KC_M03   = 0xF1,             // Switch to Layer 2
  KC_M04   = 0xF2,             // Switch to Layer 2
  KC_M05   = 0xF3,             // Switch to Layer 2
  KC_M06   = 0xF4,             // Switch to Layer 2
  KC_M07   = 0xF5,             // Switch to Layer 2

} Keycode;

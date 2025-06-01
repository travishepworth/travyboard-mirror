#ifndef DEFAULT_H
#define DEFAULT_H

#include <stdbool.h>
#include <stdint.h>

#define KEYBOARD_MODE_SPLIT false // Set to true for split keyboard mode, false for single half

enum KEYMAP_TOTALS {
  TOTAL_ROWS = 4, // Total rows for the entire keyboard (double for split keyboards)
  TOTAL_COLS = 7, // Total columns for the entire keyboard
  TOTAL_LAYERS = 4, // Total layers on this keymap
  TOTAL_ROWS_HALF = TOTAL_ROWS / 2, // Half rows for split keyboards
  TOTAL_COLS_HALF = TOTAL_COLS / 2, // Half columns for split keyboards
};

extern const uint8_t keymap_default[TOTAL_LAYERS][TOTAL_ROWS][TOTAL_COLS]; // Default keymap

#endif // DEFAULT_H


#ifndef DEFAULT_H
#define DEFAULT_H


#include <stdint.h>
enum KEYMAP_TOTALS {
  TOTAL_ROWS = 4,
  TOTAL_COLS = 7,
  TOTAL_LAYERS = 4, // Total layers on this keymap
};

extern const uint8_t keymap_default[TOTAL_LAYERS][TOTAL_ROWS][TOTAL_COLS];

#endif // DEFAULT_H

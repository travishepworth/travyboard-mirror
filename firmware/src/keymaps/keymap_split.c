#include "keymap.h"
#include "default.h"
#include <pico/types.h>
#include <string.h>
#include <stdint.h>

void construct_keymap_split(keymap_t *const keymap) {
  for (uint8_t layer = 0; layer < TOTAL_LAYERS; layer++) {
    for (uint8_t row = 0; row < TOTAL_ROWS_HALF; row++) {
      // Left half, good luck reading this
      memcpy(&keymap->full_keymap[layer][row * TOTAL_COLS_HALF], // Start at the beginning of the row
             &keymap_default[layer][row][0], // Copy entire row, starting from the beginning
             sizeof(uint8_t) * TOTAL_COLS_HALF); // Total columns for left half

      memcpy(&keymap->full_keymap[layer][(row + TOTAL_ROWS_HALF) *
                                         TOTAL_COLS_HALF], // Start at the end of left half
             &keymap_default[layer][row][TOTAL_COLS_HALF], // Copy entire row, starting halfway
             sizeof(uint8_t) * TOTAL_COLS_HALF); // Total columns for right half
    }
  }
}

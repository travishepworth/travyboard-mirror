#include "keymap.h"
#include "matrix.h"
#include <stdint.h>
#include <string.h>
#include "default.h"

void initialize_keymap_t(keymap_t *const keymap) {
  memset(keymap, 0, sizeof(*keymap));
  keymap->active_keymap = NULL;
}

void construct_keymap(keymap_t *const keymap) {
  uint8_t copy_index;
  for (uint8_t layer = 0; layer < TOTAL_LAYERS; layer++) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
      copy_index = row * MATRIX_COLS; 
      memcpy(&keymap->full_keymap[layer][copy_index], keymap_default[layer][row], MATRIX_COLS);
    }
  }
}

void initialize_keymaps(keymap_t *const keymap) {
  initialize_keymap_t(keymap);
  construct_keymap(keymap);
}

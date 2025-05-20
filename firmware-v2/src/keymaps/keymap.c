#include "keymap.h"
#include "matrix.h"
#include <stdint.h>
#include <string.h>

void initialize_keymap_modes_type(keymap_modes_t *const modes) {
  memset(modes->modes_all, 0, sizeof(modes->modes_all));
}

void initialize_keymap_type(keymap_t *const keymap, keymap_modes_t *const modes) {
  keymap->active_keymap = modes->modes_all[0];
  keymap->active_mode = 0;
  keymap->active_layer = 0;
}

void construct_keymaps(keymap_t *const keymap, keymap_modes_t *const modes) {
}

uint8_t return_keycode(keymap_t *const keymap, uint8_t row, uint8_t col) {
  if (row < MATRIX_ROWS && col < MATRIX_COLS ) {
    return keymap->active_keymap[keymap->active_layer][row][col];
  } else {
    return 0; // Invalid keycode - major problem
  }
};

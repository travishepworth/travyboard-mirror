#include "keymap.h"
#include "matrix.h"
#include <stdint.h>
#include <string.h>
#include "default.h"

void initialize_keymap_modes_type(keymap_modes_t *const modes) {
  memset(modes->modes_all, 0, sizeof(modes->modes_all));
}

void initialize_keymap_type(keymap_t *const keymap, keymap_modes_t *const modes) {
  keymap->active_keymap = modes->modes_all[0];
  keymap->active_mode = 0;
  keymap->active_layer = 0;
}

void construct_keymaps(keymap_modes_t *const modes) {
  // copy only default keymap to all modes for now -- handling later
  for (uint8_t mode = 0; mode < NUM_MODES; mode++) {
    memcpy(modes->modes_all[mode], keymap_default, sizeof(keymap_default));
  }
}

void set_mode(keymap_t *const keymap, keymap_modes_t *const modes, uint8_t mode) {
  keymap->active_keymap = modes->modes_all[mode];
}

uint8_t return_keycode(keymap_t *const keymap, uint8_t row, uint8_t col) {
  if (row < MATRIX_ROWS && col < MATRIX_COLS ) {
    return keymap->active_keymap[keymap->active_layer][row][col];
  } else {
    return 0; // Invalid keycode - major problem
  }
};

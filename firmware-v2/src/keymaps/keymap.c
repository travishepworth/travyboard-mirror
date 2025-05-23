#include "keymap.h"
#include <stdint.h>
#include <string.h>
#include "default.h"
#include "keyboard.h"

void initialize_keymap_t(keymap_t *const keymap) {
  memset(keymap, 0, sizeof(*keymap));
  keymap->active_keymap = NULL;
}

void initialize_keymaps(keymap_t *const keymap) {
  initialize_keymap_t(keymap);
  construct_keymap(keymap);
}

void construct_keymap(keymap_t *const keymap) {
  uint8_t copy_index;
  for (uint8_t layer = 0; layer < TOTAL_LAYERS; layer++) {
    for (uint8_t row = 0; row < TOTAL_ROWS; row++) {
      copy_index = row * TOTAL_COLS; 
      memcpy(&keymap->full_keymap[layer][copy_index], keymap_default[layer][row], TOTAL_COLS);
    }
  }
}

void set_layer(keymap_t *const keymap, uint8_t layer) {
  if (layer < TOTAL_LAYERS) {
    keymap->active_keymap = keymap->full_keymap[layer];
  }
}

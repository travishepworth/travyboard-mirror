#include "keymap.h"
#include <stdint.h>
#include <string.h>
#include "default.h"

void initialize_keymap_t(keymap_t *const keymap) {
  memset(keymap, 0, sizeof(*keymap));
  keymap->active_keymap = NULL;
}

void initialize_keymaps(keymap_t *const keymap) {
  initialize_keymap_t(keymap);
  construct_keymap(keymap);
}

void construct_keymap(keymap_t *const keymap) {
  for (uint8_t layer = 0; layer < TOTAL_LAYERS; layer++) {
    memcpy(keymap->full_keymap[layer], keymap_default[layer], TOTAL_ROWS * TOTAL_COLS * sizeof(uint8_t));
  }
}

void set_layer(keymap_t *const keymap, uint8_t layer) {
  if (layer < TOTAL_LAYERS) {
    keymap->active_keymap = keymap->full_keymap[layer];
  } else {
    keymap->active_keymap = keymap->full_keymap[0]; // Default to layer 0
  }
}

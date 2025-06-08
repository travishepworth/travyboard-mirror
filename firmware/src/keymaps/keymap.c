#include "keymap.h"
#include "default.h"
#include <pico/types.h>
#include <string.h>

keymap_callback_t construct_keymap;

void select_keymap_backend(void) {
  construct_keymap = KEYBOARD_MODE_SPLIT ? construct_keymap_split : construct_keymap_single;
}

void initialize_keymap_t(keymap_t *const keymap) {
  memset(keymap, 0, sizeof(*keymap));
  keymap->active_keymap = NULL;
}

void initialize_keymaps(keymap_t *const keymap) {
  initialize_keymap_t(keymap);
  select_keymap_backend(); // Select the keymap backend based on split mode
  construct_keymap(keymap);
}

void set_layer(keymap_t *const keymap, uint8_t layer) {
  if (layer < TOTAL_LAYERS) {
    keymap->active_keymap = keymap->full_keymap[layer];
  } else {
    keymap->active_keymap = keymap->full_keymap[0]; // Default to layer 0
  }
}

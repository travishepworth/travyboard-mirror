#include "keymap.h"
#include "default.h"
#include <pico/types.h>
#include <string.h>
#include <stdint.h>

void construct_keymap_single(keymap_t *const keymap) {
  for (uint8_t layer = 0; layer < TOTAL_LAYERS; layer++) {
    memcpy(keymap->full_keymap[layer], keymap_default[layer],
           TOTAL_ROWS * TOTAL_COLS * sizeof(uint8_t));
  }
}

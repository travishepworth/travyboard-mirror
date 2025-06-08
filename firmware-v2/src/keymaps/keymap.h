#ifndef KEYMAP_H
#define KEYMAP_H

#include "default.h"

#define NUM_MODES 6

typedef struct {
  uint8_t key_index; // raw index in keymap array
  uint8_t defined_layer; // which of the layers this index maps to
} layer_key_position_t;

// All index positions that hold a layer key at x layer
typedef struct {
  layer_key_position_t positions[TOTAL_ROWS * TOTAL_COLS];
  uint8_t count;
} layer_key_group_t;

typedef struct {
  layer_key_group_t layer_keys[TOTAL_LAYERS];
} layer_system_t;

typedef struct {
  uint8_t full_keymap[TOTAL_LAYERS][TOTAL_COLS * TOTAL_ROWS]; // Fully flat may be better but I like multi-dimensional arrays
  uint8_t *active_keymap; // Pointer to current layer
  layer_system_t layer_indices;
} keymap_t;

typedef void (*keymap_callback_t)(keymap_t *const keymap);
extern keymap_callback_t construct_keymap;

void select_keymap_backend();

void initialize_keymap_t(keymap_t *const keymap);
void initialize_keymaps(keymap_t *const keymap);

void set_layer(keymap_t *const keymap, uint8_t layer);

void construct_keymap_single(keymap_t *keymap);
void construct_keymap_split(keymap_t *keymap);

#endif // KEYMAP_H

#ifndef KEYMAP_H
#define KEYMAP_H

#include "default.h"
#include "matrix.h"

#define NUM_MODES 6

// typedef struct {
//   uint8_t modes_all[NUM_MODES][TOTAL_LAYERS][TOTAL_ROWS][TOTAL_COLS];
// } keymap_modes_t;

typedef struct {
  uint8_t index[TOTAL_ROWS * TOTAL_COLS];
  uint8_t total;
} layer_index_t;

typedef struct {
  layer_index_t matrix_index[TOTAL_LAYERS]; 
  uint8_t keycode[TOTAL_LAYERS];
} layer_indices_t ;

typedef struct {
  uint8_t full_keymap[TOTAL_LAYERS][TOTAL_COLS * TOTAL_ROWS]; // Fully flat may be better but I like multi-dimensional arrays
  uint8_t *active_keymap; // Pointer to current layer
  layer_indices_t layer_indices;
} keymap_t;

void initialize_keymap_t(keymap_t *const keymap);

void construct_keymap(keymap_t *const keymap);

void initialize_keymaps(keymap_t *const keymap);

void set_layer(keymap_t *const keymap, uint8_t layer);

#endif // KEYMAP_H

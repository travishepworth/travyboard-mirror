#ifndef KEYMAP_H
#define KEYMAP_H

#include "default.h"
#include "matrix.h"

#define MAX_NUM_LAYERS 8
#define NUM_MODES 6

typedef struct {
  uint8_t modes_all[NUM_MODES][MAX_NUM_LAYERS][MATRIX_ROWS][MATRIX_COLS];
} keymap_modes_t;

typedef struct {
  uint8_t index[TOTAL_ROWS * TOTAL_COLS];
  uint8_t total;
} layer_index_t;

typedef struct {
  layer_index_t matrix_index[MAX_NUM_LAYERS]; 
  uint8_t keycode[MAX_NUM_LAYERS];
} layer_indices_t ;

typedef struct {
  uint8_t full_keymap[MAX_NUM_LAYERS][MATRIX_COLS * MATRIX_ROWS]; // Fully flat may be better but I like multi-dimensional arrays
  uint8_t (*active_keymap)[MATRIX_COLS * MATRIX_ROWS]; // Pointer to current layer
  layer_indices_t layer_indices;
} keymap_t;


void initialize_keymap_modes_type(keymap_modes_t *const modes);

void initialize_keymap_type(keymap_t *const keymap);

void construct_keymap(keymap_t *const keymap);

uint8_t return_keycode(keymap_t *const keymap, uint8_t index);

#endif // KEYMAP_H

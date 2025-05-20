#ifndef KEYMAP_H
#define KEYMAP_H

#include "matrix.h"

#define NUM_LAYERS 8
#define NUM_MODES 6

// Using 4 layer arrays without malloc requires careful memory management and bounds checking
typedef struct {
  uint8_t modes_all[NUM_MODES][NUM_LAYERS][MATRIX_ROWS][MATRIX_COLS];
} keymap_modes_t;

typedef struct {
  // When assigning this variable, you must assign active_keymap to point to 
  // a specific mode. When you point active_keymap to modes_all[0], you are
  // pointing to the start of the layers array in memory for that mode.
  // Fucking awesome.
  uint8_t (*active_keymap)[MATRIX_ROWS][MATRIX_COLS];
  uint8_t active_mode;
  uint8_t active_layer;
} keymap_t;

void initialize_keymap_modes_type(keymap_modes_t *const modes);

void initialize_keymap_type(keymap_t *const keymap, keymap_modes_t *const modes);

uint8_t return_keycode(keymap_t *const keymap, uint8_t row, uint8_t col);

#endif // KEYMAP_H

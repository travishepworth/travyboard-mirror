#include "matrix.h"
#include <string.h>
#include "default.h"

// matrix.c
matrix_read_t matrix_read;
matrix_init_t matrix_init;

void select_matrix_backend() {
  matrix_read = KEYBOARD_MODE_SPLIT ? matrix_read_split : matrix_read_single;
  matrix_init = KEYBOARD_MODE_SPLIT ? matrix_init_split : matrix_init_single;
}

void matrix_clear(matrix_state_t *const state) {
  // Clear the matrix state
  memset(state->state, 0, sizeof(state->state));
  memset(state->activated_keys, 0, sizeof(state->activated_keys));
  state->total_activated_keys = 0;
};

void matrix_convert(matrix_state_t *const state) {
  for (uint8_t matrix_index = 0; matrix_index < (MATRIX_SIZE); matrix_index++) {
    if (state->state[matrix_index]) {
      state->activated_keys[state->total_activated_keys++] = matrix_index;
    }
  }
}

void matrix_trim(matrix_state_t *const state) {
  if (state->total_activated_keys > 6) {
    memset(&state->activated_keys[6], 0, (MATRIX_SIZE - 6) * sizeof(uint8_t)); // Might as well clear to the end
    state->total_activated_keys = 6; // Max keys for key report
  }
}

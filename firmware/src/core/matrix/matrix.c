#include "matrix.h"
#include <string.h>
#include "default.h"

// matrix.c
matrix_read_t matrix_read;
matrix_init_t matrix_init;

// SELECTOR: Select which matrix functions to use
void select_matrix_backend() {
  matrix_read = KEYBOARD_MODE_SPLIT ? matrix_read_split : matrix_read_single;
  matrix_init = KEYBOARD_MODE_SPLIT ? matrix_init_split : matrix_init_single;
}

// HELPER: Clear the matrix
void matrix_clear(matrix_state_t *const state) {
  // Clear the matrix state
  memset(state->state, 0, sizeof(state->state));
  memset(state->activated_keys, 0, sizeof(state->activated_keys));
  state->total_activated_keys = 0;
};

// INIT: Init function
void matrix_initialize_metadata(matrix_metadata_t *const metadata) {
  // Initialize the metadata for the matrix
  memset(metadata, 0, sizeof(*metadata));
}

// CORE: Extract indices from bool array to single array
void matrix_convert(matrix_state_t *const state) {
  memset(state->activated_keys, 0, sizeof(state->activated_keys));
  for (uint8_t matrix_index = 0; matrix_index < (MATRIX_SIZE); matrix_index++) {
    if (state->state[matrix_index]) {
      state->activated_keys[state->total_activated_keys++] = matrix_index;
    }
  }
}

// HELPER: Check if matrix state is empty
bool matrix_is_empty(matrix_state_t const *const state) {
  for (uint8_t i = 0; i < MATRIX_SIZE; i++) {
    if (state->state[i]) {
      return false;
    }
  }
  return true;
}

// HELPER: Copy entire state to other state
void matrix_copy(matrix_state_t *const target_matrix, matrix_state_t const *const source_matrix) {
  memcpy(target_matrix, source_matrix, sizeof(matrix_state_t));
};

// HELPER: Compare two states
bool matrix_state_equals(matrix_state_t const *const matrix_a, matrix_state_t const *const matrix_b) {
  return (memcmp(matrix_a->state, matrix_b->state, sizeof(matrix_a->state)) == 0);
}

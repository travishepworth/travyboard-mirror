// travmonkey
// Define functions to read keyboard matrix
#ifndef MATRIX_H
#define MATRIX_H

#include "default.h"
#include <stdbool.h>
#include <stdint.h>

#define MATRIX_SIZE (TOTAL_COLS * TOTAL_ROWS) // Total size of the matrix
#define MATRIX_HALF_SIZE (TOTAL_COLS_HALF * TOTAL_ROWS_HALF) // Half size of the matrix for split keyboards

// Define a struct to hold the state of the matrix
typedef struct {
  uint8_t state[MATRIX_SIZE]; // Hardware state
  uint8_t activated_keys[MATRIX_SIZE];
  uint8_t total_activated_keys;
} matrix_state_t;

typedef struct {
  union {
    uint8_t row_pins_full[TOTAL_ROWS]; // Full row pins for single matrix
    uint8_t row_pins_half[TOTAL_ROWS_HALF]; // Half row pins for split matrix
  };
  union {
    uint8_t col_pins_full[TOTAL_COLS]; // Full column pins for single matrix
    uint8_t col_pins_half[TOTAL_COLS_HALF]; // Half column pins for split matrix
  };
  uint8_t half; // TODO: move this to an enum
} matrix_metadata_t;

typedef void (*matrix_read_t)(matrix_state_t *const state, matrix_metadata_t *const metadata);
extern matrix_read_t matrix_read;

typedef void (*matrix_init_t)(matrix_metadata_t *const metadata);
extern matrix_init_t matrix_init;

// Function to initialize the matrix
void matrix_init_single(matrix_metadata_t *const metadata);
void matrix_init_split(matrix_metadata_t *const metadata);

void matrix_initialize_metadata(matrix_metadata_t *const metadata);

void select_matrix_backend(void);

void matrix_detect_half(matrix_metadata_t *const metadata);
void matrix_split_set_metadata(matrix_metadata_t *const metadata);
void matrix_single_set_metadata(matrix_metadata_t *const metadata);

// Function to clear the matrix state
void matrix_clear(matrix_state_t *const state);
bool matrix_is_empty(matrix_state_t const *const state);

// Functions to read the state of the matrix
void matrix_read_single(matrix_state_t *const state, matrix_metadata_t *const metadata);
void matrix_read_split(matrix_state_t *const state, matrix_metadata_t *const metadata);

// Get an array of indices of pressed keys
void matrix_convert(matrix_state_t *const state);

void matrix_trim(matrix_state_t *const state);

void matrix_concatenate(matrix_state_t *const state, const matrix_state_t *right_state);

void matrix_copy(matrix_state_t *const target_matrix, matrix_state_t const *const source_matrix);

bool matrix_state_equals(matrix_state_t const *const matrix_a, matrix_state_t const *const matrix_b);

#endif // MATRIX_H

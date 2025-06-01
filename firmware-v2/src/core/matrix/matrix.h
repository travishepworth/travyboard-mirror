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
  uint8_t state[TOTAL_COLS * TOTAL_ROWS]; // Hardware state
  uint8_t activated_keys[TOTAL_ROWS * TOTAL_COLS];
  uint8_t total_activated_keys;
} matrix_state_t;

typedef void (*matrix_read_t)(matrix_state_t *const state);
extern matrix_read_t matrix_read;

typedef void (*matrix_init_t)(void);
extern matrix_init_t matrix_init;

// Function to initialize the matrix
void matrix_init_single(void);
void matrix_init_split(void);

void select_matrix_backend(void);

// Function to clear the matrix state
void matrix_clear(matrix_state_t *const state);

// Functions to read the state of the matrix
void matrix_read_single(matrix_state_t *const state);
void matrix_read_split(matrix_state_t *const state);

// Get an array of indices of pressed keys
void matrix_convert(matrix_state_t *const state);

void matrix_trim(matrix_state_t *const state);

void matrix_concatenate(matrix_state_t *const state, const matrix_state_t *right_state);

#endif // MATRIX_H

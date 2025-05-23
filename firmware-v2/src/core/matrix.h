// travmonkey
// Define functions to read keyboard matrix
#ifndef MATRIX_H
#define MATRIX_H

#include "default.h"
#include <stdbool.h>
#include <stdint.h>

// Define a struct to hold the state of the matrix
typedef struct {
  bool state[TOTAL_COLS * TOTAL_ROWS]; // Hardware state
  uint8_t activated_keys[TOTAL_ROWS * TOTAL_COLS];
  uint8_t total_activated_keys;
} matrix_state_t;

// Function to initialize the matrix
void matrix_init(void);

// Function to clear the matrix state
void matrix_clear(matrix_state_t *const state);

// Function to read the state of the matrix
void matrix_read(matrix_state_t *const state);

// Get an array of indices of pressed keys
void matrix_convert(matrix_state_t *const state);

void matrix_trim(matrix_state_t *const state);

#endif // MATRIX_H

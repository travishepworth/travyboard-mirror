// travmonkey
// Define functions to read keyboard matrix
#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

#define MATRIX_ROWS 4 // Number of rows in the matrix
#define MATRIX_COLS 4 // Number of columns in the matrix

// Define a struct to hold the state of the matrix
typedef struct {
  uint8_t state[MATRIX_ROWS][MATRIX_COLS]; // 4x4 matrix
} matrix_state_t;

// Function to initialize the matrix
void matrix_init(void);

// Function to read the state of the matrix
void matrix_read(matrix_state_t *state);

#endif // MATRIX_H

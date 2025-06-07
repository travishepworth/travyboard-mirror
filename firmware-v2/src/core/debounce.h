// debounce.h
#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "matrix.h"
#include "pico/time.h"
#include <stdbool.h>
#include <stdint.h>

// Debounce configuration
#define DEBOUNCE_TIME_MS 5
#define TOTAL_KEYS (TOTAL_COLS * TOTAL_ROWS)

// Debounce state structure
typedef struct {
  uint8_t debounced_state[TOTAL_KEYS]; // Clean, stable key states
  uint8_t raw_state[TOTAL_KEYS];       // Last raw reading for change detection
  uint32_t release_timers[TOTAL_KEYS]; // Per-key release debounce timers
                                       // (microseconds)
} debounce_state_t;

// Function prototypes
void debounce_init(debounce_state_t *debounce);
void debounce_matrix(matrix_state_t *state, debounce_state_t *debounce);

#endif // DEBOUNCE_H

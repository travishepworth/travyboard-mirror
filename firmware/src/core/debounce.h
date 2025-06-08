// debounce.h
#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "matrix.h"
#include "pico/time.h"
#include <stdbool.h>
#include <stdint.h>

#define DEBOUNCE_TIME_MS 5
#define TOTAL_KEYS (TOTAL_COLS * TOTAL_ROWS)

typedef struct {
  uint8_t debounced_state[TOTAL_KEYS];
  uint8_t raw_state[TOTAL_KEYS];
  uint32_t release_timers[TOTAL_KEYS];
} debounce_state_t;

void debounce_init(debounce_state_t *debounce);
void debounce_matrix(matrix_state_t *state, debounce_state_t *debounce);

#endif // DEBOUNCE_H

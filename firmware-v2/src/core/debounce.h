#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "default.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool debounced;          // Last state of the key (pressed or not)
  bool raw;    // Debounced state of the key
  uint32_t last_change;
} key_state_t;

key_state_t key_matrix[TOTAL_ROWS][TOTAL_COLS]; // Matrix of key states


#endif // DEBOUNCE_H

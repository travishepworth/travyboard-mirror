// TODO: Build a rubst debounce function with a state machine
#include "debounce.h"
#include "matrix.h"
#include <pico/time.h>
#include <stdint.h>
#include <stdio.h>

#define DEBOUNCE_DELAY_MS 5 // Debounce delay in milliseconds

bool read_matrix(matrix_state_t const *state) {
  for (uint8_t index = 0; index < (TOTAL_ROWS * TOTAL_COLS); index++) {
    if (state->state[index]) {
      // If any key is pressed, return true
      return true;
    }
  }
  return false;  // replace with actual read
}

// Todo tomorrow, implement the rest of debouncing logic. Ask Jon about this.
void scan_and_debounce_matrix(matrix_state_t *matrix) {
  uint32_t now = to_ms_since_boot(get_absolute_time());

  for (int row = 0; TOTAL_ROWS > row; row++) {
    for (int col = 0; col < TOTAL_COLS; col++) {
      bool raw = read_matrix(matrix);
      key_state_t *key = &key_matrix[row][col];

      if (raw != key->raw) {
        key->raw = raw;
        key->last_change = now;
      }

      if ((now - key->last_change) >= DEBOUNCE_DELAY_MS &&
          key->debounced != key->raw) {
        key->debounced = key->raw;

        // Trigger event here
        if (key->debounced) {
          printf("Key pressed: row %d, col %d\n", row, col);
          // your_keymap_fn(row, col); // optional callback
        } else {
          printf("Key released: row %d, col %d\n", row, col);
        }
      }
    }
  }
}

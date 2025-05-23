// travmonkey
// Define functions to read keyboard matrix

#include <hardware/gpio.h>
#include <pico/time.h>
#include <pico/types.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "matrix.h"

// These will be defined elsewhere in the code, keeping them here for
// lsp sanity for now
static const uint row_pins[TOTAL_ROWS] = {0, 1, 2, 3}; // GPIO pins for rows
static const uint col_pins[TOTAL_COLS] = {4, 5, 6, 7}; // GPIO pins for columns

void matrix_init(void) {
  // Init the row pins; set them to output
  for (uint8_t pin_index = 0; pin_index < TOTAL_ROWS; pin_index++) {
    gpio_init(row_pins[pin_index]);
    gpio_set_dir(row_pins[pin_index], GPIO_OUT);
  }

  // Init the column pins; set them to input
  for (uint8_t pin_index = 0; pin_index < TOTAL_COLS; pin_index++) {
    gpio_init(col_pins[pin_index]);
    gpio_set_dir(col_pins[pin_index], GPIO_IN);
    gpio_pull_up(col_pins[pin_index]);
  }
};

void matrix_clear(matrix_state_t *const state) {
  // Clear the matrix state
  memset(state->state, 0, sizeof(state->state));
};

// This is the only function that actually interacts with hardware
// It must be as optimized as possible.
void matrix_read(matrix_state_t *const state) {
  // Row doesn't matter, but setting column here instead of in the loop is ideal
  uint8_t row;
  uint8_t col;
  uint8_t destination_index = 0;
  // Perform a scan of the matrix
  // We will pull row pins high, then read 
  for (row = 0; row < TOTAL_ROWS; row++) {
    gpio_put(row_pins[row], 1);
    sleep_us(1); // Slight delay to increase accuracy
    
    // Read every column pin to check for button presses
    for (col = 0; col < TOTAL_COLS; col++) {
      state->state[destination_index++] = gpio_get(col_pins[col]);
    }
    // Stop scanning the row
    gpio_put(row_pins[row], 0);
  }
};

void matrix_convert(matrix_state_t *const state) {
  for (uint8_t matrix_index = 0; matrix_index < (TOTAL_ROWS * TOTAL_COLS); matrix_index++) {
    if (state->state[matrix_index]) {
      state->activated_keys[state->total_activated_keys++] = matrix_index;
    }
  }
}

void matrix_trim(matrix_state_t *const state) {
  if (state->total_activated_keys > 6) {
    size_t array_size = TOTAL_ROWS * TOTAL_COLS;
    memset(&state->activated_keys[6], 0, (array_size - 6) * sizeof(uint8_t)); // Might as well clear to the end
    state->total_activated_keys = 6; // Max keys for key report
  }
}

// travmonkey
// Define functions to read keyboard matrix

#include <pico/time.h>
#include <pico/types.h>
#include <stddef.h>
#include <stdint.h>
#include "matrix.h"
#include <hardware/gpio.h>

// These will be defined elsewhere in the code, keeping them here for
// lsp sanity for now
// TODO, make this support split keyboards (even scuffed fow now)
static const uint row_pins[TOTAL_ROWS] = {2, 3, 13, 15}; // GPIO pins for rows
static const uint col_pins[TOTAL_COLS] = {4, 7, 8, 9, 10, 11, 12}; // GPIO pins for columns

void matrix_init_single(void) {
  // Init the row pins; set them to output
  for (uint8_t pin_index = 0; pin_index < TOTAL_ROWS; pin_index++) {
    gpio_init(row_pins[pin_index]);
    gpio_set_dir(row_pins[pin_index], GPIO_OUT);
  }

  // Init the column pins; set them to input
  for (uint8_t pin_index = 0; pin_index < TOTAL_COLS; pin_index++) {
    gpio_init(col_pins[pin_index]);
    gpio_set_dir(col_pins[pin_index], GPIO_IN);
    gpio_pull_down(col_pins[pin_index]);
  }
};

void matrix_read_single(matrix_state_t *const state) {
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

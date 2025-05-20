// travmonkey
// Define functions to read keyboard matrix

#include <hardware/gpio.h>
#include <pico/time.h>
#include <pico/types.h>
#include <stdint.h>
#include "matrix.h"

static const uint row_pins[MATRIX_ROWS] = {0, 1, 2, 3}; // GPIO pins for rows
static const uint col_pins[MATRIX_COLS] = {4, 5, 6, 7}; // GPIO pins for columns

void matrix_init(void) {
  // Init the row pins; set them to output
  for (uint8_t pin_index = 0; pin_index < MATRIX_ROWS; pin_index++) {
    gpio_init(pin_index);
    gpio_set_dir(pin_index, GPIO_OUT);
  }

  // Init the column pins; set them to input
  for (uint8_t pin_index = 0; pin_index < MATRIX_COLS; pin_index++) {
    gpio_init(pin_index);
    gpio_set_dir(pin_index, GPIO_IN);
    gpio_pull_up(pin_index);
  }
};

void matrix_read(matrix_state_t *const state) {
  // Perform a scan of the matrix
  // We will pull row pins high, then read 
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    gpio_put(row_pins[row], 1);
    sleep_us(1); // Slight delay to increase accuracy
    
    // Read every column pin to check for button presses
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
      if (gpio_get(col_pins[col])) {
        state->state[row][col] = 1;
      }
    }
    
    // Stop scanning the row
    gpio_put(row_pins[row], 0);
  }
};

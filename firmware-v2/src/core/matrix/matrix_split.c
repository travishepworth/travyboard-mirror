#include <pico/time.h>
#include <pico/types.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "default.h"
#include "matrix.h"
#include <hardware/gpio.h>

static const uint row_pins[TOTAL_ROWS] = {28, 11, 10, 9, 2, 3, 13, 15}; // GPIO pins for rows
static const uint col_pins[TOTAL_COLS] = {13, 14, 15, 26, 27, 12, 29, 4, 7, 8, 9, 10, 11, 12}; // GPIO pins for columns

void matrix_init_split(void) {
  // Init the row pins; set them to output
  uint8_t row_start_index;
  uint8_t row_end_index;
  uint8_t col_start_index;
  uint8_t col_end_index;
  // TODO: Make this work for split
  // if (KEYBOARD_MODE_SPLIT) {
  row_start_index = (TOTAL_ROWS / 2); // Start from the middle for right half
  row_end_index = TOTAL_ROWS; // End at the end of the array
  col_start_index = (TOTAL_COLS / 2); // Start from the middle for right half
  col_end_index = TOTAL_COLS; // End at the end of the array
  // } else {
  //   row_start_index = 0; // Start from the beginning for left half
  //   row_end_index = (TOTAL_ROWS / 2); // End at the middle for left half
  //   col_start_index = 0; // Start from the beginning for left half
  //   col_end_index = (TOTAL_COLS / 2); // End at the middle for left half
  // }
  for (uint8_t pin_index = row_start_index; pin_index < row_end_index; ++pin_index) {
    gpio_init(row_pins[pin_index]);
    gpio_set_dir(row_pins[pin_index], GPIO_OUT);
  }

  // Init the column pins; set them to input
  for (uint8_t pin_index = col_start_index; pin_index < col_end_index; ++pin_index) {
    gpio_init(col_pins[pin_index]);
    gpio_set_dir(col_pins[pin_index], GPIO_IN);
    gpio_pull_down(col_pins[pin_index]);
  }
};

void matrix_read_split(matrix_state_t *const state) {
  // Row doesn't matter, but setting column here instead of in the loop is ideal
  uint8_t row_start_index;
  uint8_t row_end_index;
  uint8_t col_start_index;
  uint8_t col_end_index;
  // if (KEYBOARD_MODE_SPLIT) {
  row_start_index = (TOTAL_ROWS / 2); // Start from the middle for right half
  row_end_index = TOTAL_ROWS; // End at the end of the array
  col_start_index = (TOTAL_COLS / 2); // Start from the middle for right half
  col_end_index = TOTAL_COLS; // End at the end of the array
  // } else {
  //   start_index = 0; // Start from the beginning for left half
  //   end_index = (TOTAL_ROWS / 2); // End at the middle for left half
  // }
  uint8_t row;
  uint8_t col;
  uint8_t destination_index = 0;
  // Perform a scan of the matrix
  // We will pull row pins high, then read 
  for (row = row_start_index; row < row_end_index; ++row) {
    gpio_put(row_pins[row], 1);
    sleep_us(1); // Slight delay to increase accuracy
    
    // Read every column pin to check for button presses
    for (col = col_start_index; col < col_end_index; ++col) {
      state->state[destination_index++] = gpio_get(col_pins[col]);
    }
    // Stop scanning the row
    gpio_put(row_pins[row], 0);
  }
};

void matrix_concatenate(matrix_state_t *const state, const matrix_state_t *right_state) {
  // dest has room for 2 * HALF_SIZE
  // Append right_state[0..HALF_SIZE-1] to state[HALF_SIZE..(2*HALF_SIZE)-1]
  memcpy(&state->state[MATRIX_HALF_SIZE], right_state->state, MATRIX_HALF_SIZE);
}

#include <pico/time.h>
#include <pico/types.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "default.h"
#include "device/usbd.h"
#include "matrix.h"
#include <hardware/gpio.h>

static const uint8_t row_pins[TOTAL_ROWS] = {28, 11, 10, 9, 2, 3, 13, 15}; // GPIO pins for rows
static const uint8_t col_pins[TOTAL_COLS] = {13, 14, 15, 26, 27, 12, 29, 4, 7, 8, 9, 10, 11, 12}; // GPIO pins for columns

void matrix_init_split(matrix_metadata_t *const metadata) {
  matrix_split_set_metadata(metadata); // Set the metadata for split matrix
  for (uint8_t pin_index = 0; pin_index < TOTAL_ROWS_HALF; ++pin_index) {
    gpio_init(metadata->row_pins_half[pin_index]);
    gpio_set_dir(metadata->row_pins_half[pin_index], GPIO_OUT);
  }

  // Init the column pins; set them to input
  for (uint8_t pin_index = 0; pin_index < TOTAL_COLS_HALF; ++pin_index) {
    gpio_init(metadata->col_pins_half[pin_index]);
    gpio_set_dir(metadata->col_pins_half[pin_index], GPIO_IN);
    gpio_pull_down(metadata->col_pins_half[pin_index]);
  }
};

void matrix_split_set_metadata(matrix_metadata_t *const metadata) {
  // Initialize the metadata for split matrix
  matrix_initialize_metadata(metadata); // Clear the metadata struct
  matrix_detect_half(metadata); // Detect which half of the keyboard this is
  if (metadata->half == RIGHT_HALF) { // Right half
    memcpy(metadata->row_pins_half, &row_pins[TOTAL_ROWS_HALF], sizeof(metadata->row_pins_half));
    memcpy(metadata->col_pins_half, &col_pins[TOTAL_COLS_HALF], sizeof(metadata->col_pins_half));

  } else { // Left half
    memcpy(metadata->row_pins_half, row_pins, sizeof(metadata->row_pins_half));
    memcpy(metadata->col_pins_half, col_pins, sizeof(metadata->col_pins_half));
  }
}

// TODO: Make this work without usb detection
// Also, add a initialize function for the struct
void matrix_detect_half(matrix_metadata_t *const metadata) {
  metadata->half = RIGHT_HALF;
  // if (tud_connected()) {
  //   metadata->half = RIGHT_HALF;
  // } else {
  //   metadata->half = LEFT_HALF;
  // }
}

void matrix_read_split(matrix_state_t *const state, matrix_metadata_t *const metadata) {
  // Row doesn't matter, but setting column here instead of in the loop is ideal
  uint8_t row;
  uint8_t col;
  uint8_t destination_index = 0;
  // Perform a scan of the matrix
  // We will pull row pins high, then read 
  for (row = 0; row < TOTAL_ROWS_HALF; ++row) {
    gpio_put(metadata->row_pins_half[row], 1);
    sleep_us(1); // Slight delay to increase accuracy
    
    // Read every column pin to check for button presses
    for (col = 0; col < TOTAL_COLS_HALF; ++col) {
      state->state[destination_index++] = gpio_get(metadata->col_pins_half[col]);
    }
    // Stop scanning the row
    gpio_put(metadata->row_pins_half[row], 0);
  }
};

void matrix_concatenate(matrix_state_t *const state, const matrix_state_t *right_state) {
  // dest has room for 2 * HALF_SIZE
  // Append right_state[0..HALF_SIZE-1] to state[HALF_SIZE..(2*HALF_SIZE)-1]
  memcpy(&state->state[MATRIX_HALF_SIZE], right_state->state, MATRIX_HALF_SIZE);
}

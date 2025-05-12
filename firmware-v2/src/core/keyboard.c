// travmonkey
// Process array into keycodes and store in struct
// Access them later with the memory address

#include "keyboard.h"
#include "matrix.h"
#include <pico/types.h>
#include <stdint.h>
#include <string.h>

void keyboard_init(void) {
  // Initialize the keyboard
  // Need layer and debouncing init too
  matrix_init();
}

void process_matrix(keycode_report_t *report) {
  matrix_state_t state;

  // For safety; check for a null pointer
  if (report == NULL) {
    return;
  }

  // Clear last report by setting memory to 0
  memset(report->keycodes, 0, MAX_KEYCODES);
  report->count = 0;

  // Get a scan of the matrix
  matrix_read(&state);

  // TODO, apply debouncing and layer handing

  // Iterate through the matrix
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
      // Read byte stored in memory
      if (state.state[row][col]) {
        // Get keycode from keymap
        uint8_t *keycode = NULL;
        if (keycode != NULL && report->count < MAX_KEYCODES) {
          // Update keycode to index of count
          report->keycodes[report->count] = *keycode;
          report->count++; // Increment count
        }
      }
    }
  }
}

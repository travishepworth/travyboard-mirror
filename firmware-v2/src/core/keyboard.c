// travmonkey
// Process array into keycodes and store in struct
// Access them later with the memory address

#include "keyboard.h"
#include "matrix.h"
#include <pico/types.h>
#include <stdint.h>
#include <string.h>
#include <keymap.h>

void keyboard_init(keymap_modes_t *const modes, keymap_t *const keymap) {
  // Initialize the keyboard
  // Need layer and debouncing init too
  void initialize_keymap_modes_type(keymap_modes_t *const modes);
  void initialize_keymap_type(keymap_t *const keymap, keymap_modes_t *const modes);
  void matrix_init();
}

void process_matrix(keycode_report_t *report, keymap_t *const keymap) {
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
        uint8_t keycode = return_keycode(keymap, row, col);
        if (keycode != 0 && report->count < MAX_KEYCODES) {
          // Update keycode to index of count
          report->keycodes[report->count] = keycode;
          report->count++; // Increment count
        }
      }
    }
  }
}

void send_keyboard_report(keycode_report_t *report) {
}

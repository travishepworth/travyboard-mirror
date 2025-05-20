// travmonkey
// Process array into keycodes and store in struct
// Access them later with the memory address

#include "keyboard.h"
#include "class/hid/hid_device.h"
#include "matrix.h"
#include <pico/types.h>
#include <stdint.h>
#include <string.h>
#include <keymap.h>
#include "layer_processor.h"
#include "usb_descriptors.h"

void keyboard_init(keymap_modes_t *const modes, keymap_t *const keymap, mod_keys_t *const layer_and_mode) {
  // Initialize the keyboard
  // Need layer and debouncing init too
  initialize_keymap_modes_type(modes);
  initialize_keymap_type(keymap, modes);
  construct_keymaps(modes);
  initialize_layers(layer_and_mode); // Initialize the layer and mode keys
  matrix_init();
}

void process_matrix(keycode_report_t *report, keymap_t *keymap, mod_keys_t *layer_and_mode) {
  matrix_state_t state;
  matrix_clear(&state); // Clear the memory of the matrix state to be safe

  // For safety; check for a null pointer
  if (report == NULL) {
    return;
  }

  // Clear last report by setting memory to 0
  memset(report->keycodes, 0, MAX_KEYCODES);
  report->count = 0;

  // Get a scan of the matrix
  matrix_read(&state);
  set_layer_and_mode(&state, keymap, layer_and_mode);

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

void send_keyboard_report(keycode_report_t *const report) {
  if (tud_hid_ready()) {
    // Send the report to the host
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, report->keycodes);
  }
  memcpy(report->previous_keycodes, report->keycodes, MAX_KEYCODES);
  memset(report->keycodes, 0, MAX_KEYCODES); // Clear the keycodes for the next report
}

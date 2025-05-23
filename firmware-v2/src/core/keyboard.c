// travmonkey
// Process array into keycodes and store in struct
// Access them later with the memory address

#include "keyboard.h"
#include "class/hid/hid_device.h"
#include "matrix.h"
#include <keymap.h>
#include <pico/types.h>
#include <stdint.h>
#include <string.h>
#include "usb_descriptors.h"
#include "layer_processor.h"

void keyboard_init(keymap_t *const keymap,
                   layer_keys_t *const layer_info) {
  // Initialize the keyboard
  initialize_keymaps(keymap);
  initialize_layers(layer_info);
  store_layer_indices(keymap, layer_info);
  matrix_init(); // Init gpio pins for reading and writing
}

void set_keycodes(keymap_t const *const keymap, keycode_report_t *const report, matrix_state_t const *const state) {
  for (uint8_t index = 0; index < MAX_KEYCODES; index++) {
    if (state->activated_keys[index] < (TOTAL_COLS * TOTAL_ROWS)) {
      report->keycodes[index] = keymap->active_keymap[state->activated_keys[index]];
    }
  }
}

void process_matrix(keycode_report_t *report, keymap_t *keymap) {
  matrix_state_t state;
  matrix_clear(&state); // Clear the memory of the matrix state to be safe

  // Clear last report by setting memory to 0
  memset(report->keycodes, 0, MAX_KEYCODES);
  report->count = 0;

  matrix_read(&state); // Scan matrix
  matrix_convert(&state); // Extract array of indices from scan

  set_layer(keymap, return_layer(&state, &keymap->layer_indices));
  matrix_trim(&state);

  set_keycodes(keymap, report, &state);

  // TODO, apply debouncing and layer handing
  // TODO, implement way to scan half a matrix for split keyboards
  // I think of two ways to do this; make the structs for each half and do them
  // independently OR, I was thinking to just define the pins left->right.
  // Divide pins by two. Left half maxes at half Right half starts half way
  // through. Gonna have to implement UART first Another IDEA: Create another
  // variable in state, send state over UART, and concat them
}

void send_keyboard_report(keycode_report_t *const report) {
  if (tud_hid_ready()) {
    // Send the report to the host
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, report->keycodes);
  }
  // TODO, implement modifier keys
  // TODO, implement consumer keys
  // TODO, check for different layered keys on the last report, and change
  // the next key to be sent to match.
  // Clear the keycodes for the next report
  memcpy(report->previous_keycodes, report->keycodes, MAX_KEYCODES);
  memset(report->keycodes, 0, MAX_KEYCODES);
}

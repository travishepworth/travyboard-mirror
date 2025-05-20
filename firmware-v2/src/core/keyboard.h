#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "keymap.h"
#include "layer_processor.h"
#include <stdint.h>

#define MAX_KEYCODES 6

typedef struct {
  uint8_t keycodes[MAX_KEYCODES]; // Array to hold keycodes
  uint8_t previous_keycodes[MAX_KEYCODES]; // Previous keycodes for debouncing
  uint8_t modifiers[MAX_KEYCODES]; // Modifiers (e.g., Shift, Control)
  uint8_t count; // Number of keycodes in the array
} keycode_report_t;

void keyboard_init(keymap_modes_t *const modes, keymap_t *const keymap, mod_keys_t *const layer_and_mode);

void process_matrix(keycode_report_t *report, keymap_t *const keymap, mod_keys_t *const layer_and_mode);

void send_keyboard_report(keycode_report_t *report);

#endif // KEYBOARD_H

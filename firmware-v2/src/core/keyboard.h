#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "keymap.h"
#include "layer_processor.h"
#include <stdint.h>

#define MAX_KEYCODES 6

typedef struct {
  uint8_t keycodes[MAX_KEYCODES];          // Array to hold keycodes
  uint8_t previous_keycodes[MAX_KEYCODES]; // Previous keycodes for debouncing
  uint8_t modifiers[MAX_KEYCODES];         // Modifiers (e.g., Shift, Control)
  uint8_t count;                           // Number of keycodes in the array
} keycode_report_t;

void keyboard_init(keymap_t *const keymap,
                   layer_keys_t *const layer_and_mode);

void process_matrix(keycode_report_t *report, keymap_t *keymap);

void send_keyboard_report(keycode_report_t *report);

void set_keycodes(keymap_t const *const keymap, keycode_report_t *const report, matrix_state_t const *const state);

#endif // KEYBOARD_H

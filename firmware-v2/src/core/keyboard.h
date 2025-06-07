#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "keymap.h"
#include "layer_processor.h"
#include <stdint.h>

#define MAX_KEYCODES 6

typedef struct {
  uint8_t keycodes[MAX_KEYCODES];
  uint8_t indecies[MAX_KEYCODES];
  uint8_t count;
} locked_keycodes_t;

typedef struct {
  uint8_t keycodes[MAX_KEYCODES];
  uint8_t count;
  uint8_t modifier;
  uint8_t media_key;
  uint8_t media_key_active;

  locked_keycodes_t previous_keycodes;
  locked_keycodes_t current_keycodes;
} keycode_report_t;

void keyboard_init(keymap_t *const keymap, matrix_metadata_t *const metadata);

void process_matrix(keycode_report_t *report, keymap_t *keymap,
                    matrix_metadata_t *const metadata);

void send_keyboard_report(keycode_report_t *report);

void lock_keycode(locked_keycodes_t *const previous_report, uint8_t keycode,
                  uint8_t current_index);

uint8_t set_modifier_bit(uint8_t keycode);

bool is_layer_key(uint8_t keycode);

uint8_t check_locked_index(locked_keycodes_t *const previous_report,
                           uint8_t current_index);

void key_handler(keycode_report_t *const report, uint8_t keycode);

void set_keycodes(keymap_t const *const keymap, keycode_report_t *const report,
                  matrix_state_t const *const state);

#endif // KEYBOARD_H

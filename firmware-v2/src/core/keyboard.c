// travmonkey
// Process array into keycodes and store in struct
// Access them later with the memory address
#include "class/hid/hid_device.h"
#include <pico/types.h>
#include <stdint.h>
#include <string.h>

#include "keymap_shorthands.h"
#include "matrix.h"
#include "keymap.h"
#include "keyboard.h"
#include "usb_descriptors.h"
#include "layer_processor.h"

void keyboard_init(keymap_t *const keymap,
                   layer_keys_t *const layer_info, matrix_metadata_t *const metadata) {
  // Initialize the keyboard
  initialize_keymaps(keymap);
  initialize_layers(layer_info);
  store_layer_indices(keymap, layer_info);
  select_matrix_backend();
  matrix_init(metadata); // Init gpio pins for reading and writing
}

void initialize_report_t(keycode_report_t *const report) {
  memset(report, 0, sizeof(keycode_report_t)); // Clear the report struct
}

void clear_current_report(keycode_report_t *const report) {
  memset(report->keycodes, 0, MAX_KEYCODES);
  report->count = 0;
  report->modifier = 0;
  report->media_key = 0;
  // media_key_active is static and should not be cleared
}

uint8_t check_locked_index(locked_keycodes_t *const previous_report, uint8_t current_index) {
  for (uint8_t index = 0; index < MAX_KEYCODES; ++index) {
    if (current_index == previous_report->indecies[index]) {
      return previous_report->keycodes[index];
    }
  }
  return 0;
}

void lock_keycode(locked_keycodes_t *const previous_report, uint8_t keycode, uint8_t current_index) {
  previous_report->keycodes[previous_report->count] = keycode;
  previous_report->indecies[previous_report->count++] = current_index;
}

void set_keycodes(keymap_t const *const keymap, keycode_report_t *const report, matrix_state_t const *const state) {
  for (uint8_t index = 0; index < state->total_activated_keys; ++index) {
    if (state->activated_keys[index] < MATRIX_SIZE) {
      lock_keycode(&report->current_keycodes, keymap->active_keymap[state->activated_keys[index]], index);
      uint8_t pending_locked_keycode = check_locked_index(&report->previous_keycodes, state->activated_keys[index]);
      if (pending_locked_keycode == 0) {
      // if (true) {
        key_handler(report, keymap->active_keymap[state->activated_keys[index]]);
      } else {
        key_handler(report, pending_locked_keycode);
      }
    }
  }
}

void reset_locked_keycodes(keycode_report_t *const report) {
  memset(&report->previous_keycodes, 0, sizeof(locked_keycodes_t));
  memcpy(&report->previous_keycodes, &report->current_keycodes, sizeof(locked_keycodes_t));
  memset(&report->current_keycodes, 0, sizeof(locked_keycodes_t));
}

uint8_t set_modifier_bit(uint8_t keycode) {
  switch (keycode) {
    case KC_LCTL: return 0;
    case KC_LSFT: return 1;
    case KC_LALT: return 2;
    case KC_LGUI: return 3;
    case KC_RCTL: return 4;
    case KC_RSFT: return 5;
    case KC_RALT: return 6;
    case KC_RGUI: return 7;
    default:      return 0xFF;
  }
}

void key_handler(keycode_report_t *const report, uint8_t keycode) {
  uint8_t modifier_bit = set_modifier_bit(keycode);
  if (modifier_bit != 0xFF) {
    report->modifier |= (1 << modifier_bit);
    return; // No need to add to keycodes array
  }
  switch (keycode) {
    case KC_BRID:
    case KC_BRIP:
    case KC_MPRV:
    case KC_MNXT:
    case KC_MPLY:
      report->media_key = keycode;
      report->media_key_active = true;
      break;
    default:
      if (report->count < MAX_KEYCODES) {
        report->keycodes[report->count++] = keycode;
        return;
      }
  }
}

void send_keyboard_report(keycode_report_t *const report) {
  if (tud_hid_ready()) {
    // todo: move to loop and array to support multiple media keys
    if (!report->media_key_active && report->media_key != 0) {
      tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &report->media_key, sizeof(report->media_key));
      report->media_key_active = true;
    } else if (report->media_key_active && report->media_key == 0) {
      tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &report->media_key, sizeof(report->media_key));
      report->media_key_active = false;
    }
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, report->modifier, report->keycodes);
  }
  reset_locked_keycodes(report);
  clear_current_report(report);
}

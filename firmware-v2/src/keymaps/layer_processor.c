#include "layer_processor.h"
#include "keymap.h"
#include "matrix.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void initialize_layers(mod_keys_t *const layer) {
  // Init memory to zeroes
  memcpy(layer->layer_keycodes, 0, NUM_POSSIBLE_LAYERS);
  memcpy(layer->mode_keycodes, 0, NUM_POSSIBLE_MODES);
  memcpy(layer->held_layers, 0, NUM_POSSIBLE_LAYERS);
  memcpy(layer->held_modes, 0, NUM_POSSIBLE_MODES);

  uint8_t temp_layers[NUM_POSSIBLE_LAYERS] = {
    LAYER_DEFAULT,
    LAYER_KEY_M01,
    LAYER_KEY_M02,
    LAYER_KEY_M03,
    LAYER_KEY_M04,
    LAYER_KEY_M05,
    LAYER_KEY_M06,
    LAYER_KEY_M07,
  };

  uint8_t temp_modes[NUM_POSSIBLE_MODES] = {
    MODE_KEY_M01,
    MODE_KEY_M02,
    MODE_KEY_M03,
    MODE_KEY_M04,
    MODE_KEY_M05,
    MODE_KEY_M06
  };

  // Copy in Key references
  memcpy(layer->layer_keycodes, temp_layers, sizeof(temp_layers));
  memcpy(layer->mode_keycodes, temp_modes, sizeof(temp_modes));
}

void clear_layer_mode_keys(mod_keys_t *const layer) {
  // Clear the layer keys
  memset(layer->held_layers, 0, NUM_POSSIBLE_LAYERS);
  memset(layer->held_modes, 0, NUM_POSSIBLE_MODES);
}

// Kinda illegally long and way too many variables passed through. Effecient but may refactor later
layer_mode_t check_and_store_layer_mode(mod_keys_t *const layer, keymap_t *const keymap, uint8_t row, uint8_t col, int layer_index, int mode_index) {
  for (uint8_t layer_index = 0; layer_index < NUM_POSSIBLE_LAYERS; layer_index++) {
    for (uint8_t code = 0; code < NUM_POSSIBLE_LAYERS; code++) {
      if (layer->layer_keycodes[code] == keymap->active_keymap[layer_index][row][col]) {
        // for loops provide bounds checking; safe to add to array
        layer->held_layers[code] = keymap->active_keymap[layer_index][row][col];
        return SET_LAYER;
      } else {
        continue;
      }
    }
    for (uint8_t code = 0; code < NUM_POSSIBLE_MODES; code++) {
      if (layer->held_modes[code] == keymap->active_keymap[layer_index][row][col]) {
        // for loops provide bounds checking; safe to add to array
        layer->held_modes[code] = keymap->active_keymap[layer_index][row][col];
        return SET_MODE;
      } else {
        continue;
      }
    }
  }
  return SET_NONE;
}

void calculate_and_set_layer(keymap_t *const keymap, mod_keys_t *const layer, int layer_index) {
  bool layers_present[NUM_POSSIBLE_LAYERS] = {false};
  for (uint8_t index = 0; index < layer_index; index++) {
    for (uint8_t index_all = 0; index_all < NUM_POSSIBLE_LAYERS; index_all++) {
      if (layer->held_layers[index] == layer->layer_keycodes[index_all]) {
        // Set layer keys to their respective space in the array for checking
        layers_present[index_all] = true;
      }
    }
  }
  layers_present[0] = true; // Always set default layer
  for (uint8_t layer_id = 0; layer_id < NUM_POSSIBLE_LAYERS; layer_id++) {
    if (layers_present[layer_id]) {
      keymap->active_layer = layer_id;
      // Loop again
      continue;
    } else {
      // Next layer not present, Leave function as variable is set
      return;
    }
  }
}

void set_layer_and_mode(matrix_state_t *const state, keymap_t *keymap, mod_keys_t *const layer) {
  int layer_index = 0;
  int mode_index = 0;
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
      if (state->state[row][col]) {
        layer_mode_t result = check_and_store_layer_mode(layer, keymap, row, col, layer_index, mode_index);
        if (result == SET_LAYER) {
          layer_index++;
        } else if (result == SET_MODE) {
          mode_index++;
        } else {
          continue;
        }
      }
    }
  }
  // Set the layer and mode
  calculate_and_set_layer(keymap, layer, layer_index);
  clear_layer_mode_keys(layer);
  // TODO handle mode switching
}

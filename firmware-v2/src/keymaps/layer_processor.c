#include "layer_processor.h"
#include "default.h"
#include "keymap.h"
#include "matrix.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void initialize_layers(layer_keys_t *const layer) {
  // Init memory to zeroes
  memcpy(layer->layer_keycodes, 0, NUM_POSSIBLE_LAYERS);

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

  // uint8_t temp_modes[NUM_POSSIBLE_MODES] = {
  //   MODE_KEY_M01,
  //   MODE_KEY_M02,
  //   MODE_KEY_M03,
  //   MODE_KEY_M04,
  //   MODE_KEY_M05,
  //   MODE_KEY_M06
  // };

  // Copy in Key references
  memcpy(layer->layer_keycodes, temp_layers, sizeof(temp_layers));
}

// This took awhile to come too, but I think it is pretty good now
void store_layer_indices(keymap_t *const keymap, layer_keys_t const *const layer_info) {
  // Set pointer for readability
  layer_index_t *keycode_indices;

  for (uint8_t layer = 0; layer < NUM_POSSIBLE_LAYERS; layer++) {
    for (uint8_t matrix_index = 0; matrix_index < (TOTAL_COLS * TOTAL_ROWS); matrix_index++) {
      for (uint8_t code = 0; code < NUM_POSSIBLE_LAYERS; code++) {
        if (layer_info->layer_keycodes[code] == keymap->full_keymap[layer][matrix_index]) {

          // Prevent re-writing the keycode
          if (keymap->layer_indices.keycode[code] != layer_info->layer_keycodes[code]) {
            keymap->layer_indices.keycode[code] = layer_info->layer_keycodes[code];
          }

          // Add index to array matching the key
          keycode_indices = &keymap->layer_indices.matrix_index[code];
          if (keycode_indices->total < (TOTAL_COLS * TOTAL_ROWS)) {
            keycode_indices->index[keycode_indices->total++] = matrix_index;
          }
        }
      }
    }
  }
}

uint8_t return_layer(matrix_state_t const *const state, layer_indices_t *const layer_indices) {
  uint8_t highest_layer = 0; // Default to layer 0

  // Loop through all activated keys
  for (uint8_t activated_index = 0; activated_index < state->total_activated_keys; activated_index++) {
    uint8_t key_index = state->activated_keys[activated_index];
    
    // Check each layer for a matching index
    for (uint8_t layer = 0; layer < NUM_POSSIBLE_LAYERS; layer++) {
      for (uint8_t stored_index = 0; stored_index < layer_indices->matrix_index[layer].total; stored_index++) {
        if (layer_indices->matrix_index[layer].index[stored_index] == key_index) {
          // Update highest_layer if this layer is higher
          if (layer > highest_layer) {
            highest_layer = layer;
          }
          break; // Found a match in this layer, move to next key
        }
      }
    }
  }
  return highest_layer;
}

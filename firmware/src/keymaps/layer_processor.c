#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "layer_processor.h"
#include "default.h"
#include "keymap.h"
#include "matrix.h"

void add_layer_key_position(layer_system_t *system, uint8_t key_index, uint8_t defined_layer, uint8_t target_layer) {
  if (target_layer >= TOTAL_LAYERS) return;
  
  layer_key_group_t *group = &system->layer_keys[target_layer];
  
  if (group->count < TOTAL_ROWS * TOTAL_COLS) {
    group->positions[group->count].key_index = key_index;
    group->positions[group->count++].defined_layer = defined_layer;
  }
}

void store_layer_indices(keymap_t *const keymap) {
  // Clear existing layer indices
  memset(&keymap->layer_indices, 0, sizeof(layer_system_t));
  
  // Scan through all layers and positions
  for (uint8_t layer = 0; layer < TOTAL_LAYERS; layer++) {
    for (uint8_t matrix_index = 0; matrix_index < (TOTAL_COLS * TOTAL_ROWS); matrix_index++) {
      uint8_t keycode = keymap->full_keymap[layer][matrix_index];
      
      // Check if this is a layer key (within our layer keycode range)
      // NOTE: do not define a layer key targeting a higher layer than your total layers
      if (keycode >= LAYER_BASE && keycode < LAYER_BASE + TOTAL_LAYERS) {
        uint8_t target_layer = keycode - LAYER_BASE;
        
        // Add this position to the appropriate layer group
        add_layer_key_position(&keymap->layer_indices, matrix_index, layer, target_layer);
      }
    }
  }
}

uint8_t get_active_layer(matrix_state_t const state, layer_system_t const *layer_system) {
  uint8_t current_layer = 0;
  bool layer_changed = true;
  
  while (layer_changed) {
    layer_changed = false;
    uint8_t highest_available = current_layer;
    
    // Check each possible target layer (skip layer 0 since we start there)
    for (uint8_t target_layer = 1; target_layer < TOTAL_LAYERS; ++target_layer) {
      if (target_layer <= highest_available) continue; // Only check higher layers
      
      // Check if any key that activates this target layer is pressed
      bool layer_key_pressed = false;
      for (uint8_t pos = 0; pos < layer_system->layer_keys[target_layer].count; ++pos) {
        layer_key_position_t key_pos = layer_system->layer_keys[target_layer].positions[pos];
        
        // Only consider keys defined on the current layer
        if (key_pos.defined_layer != current_layer) continue;
        
        // Check if this key is pressed
        for (uint8_t i = 0; i < state.total_activated_keys; ++i) {
          if (state.activated_keys[i] == key_pos.key_index) {
            layer_key_pressed = true;
            break;
          }
        }
        
        if (layer_key_pressed) break;
      }
      
      if (layer_key_pressed && target_layer > highest_available) {
        highest_available = target_layer;
        layer_changed = true;
      }
    }
    
    current_layer = highest_available;
  }
  
  return current_layer;
}

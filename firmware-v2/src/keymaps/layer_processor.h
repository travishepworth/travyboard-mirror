#ifndef LAYER_PROCESSOR_H
#define LAYER_PROCESSOR_H

#include <stdbool.h>
#include <stdint.h>

#include "keymap.h"
#include "matrix.h"

#define NUM_POSSIBLE_LAYERS 8
#define LAYER_BASE 240 // Out of scope for keycodes
#define LAYER_DEFAULT 240
#define LAYER_KEY_M01 241
#define LAYER_KEY_M02 242
#define LAYER_KEY_M03 243
#define LAYER_KEY_M04 244
#define LAYER_KEY_M05 245
#define LAYER_KEY_M06 246
#define LAYER_KEY_M07 247

#define NUM_POSSIBLE_MODES 6
#define MODE_KEY_M01 0xF9
#define MODE_KEY_M02 0xFA
#define MODE_KEY_M03 0xFB
#define MODE_KEY_M04 0xFC
#define MODE_KEY_M05 0xFD
#define MODE_KEY_M06 0xFE

void add_layer_key_position(layer_system_t *system, uint8_t key_index, uint8_t defined_layer, uint8_t target_layer);

void store_layer_indices(keymap_t *const keymap);

uint8_t get_active_layer(matrix_state_t const state, layer_system_t const *layer_system);

#endif // LAYER_PROCESSOR_H

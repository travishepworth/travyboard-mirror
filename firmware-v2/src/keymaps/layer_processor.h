#ifndef LAYER_PROCESSOR_H
#define LAYER_PROCESSOR_H

#include "keymap.h"
#include "matrix.h"
#include <stdbool.h>
#include <stdint.h>

#define NUM_POSSIBLE_LAYERS 8
#define LAYER_DEFAULT 0xFF
#define LAYER_KEY_M01 0xF0
#define LAYER_KEY_M02 0xF1
#define LAYER_KEY_M03 0xF2
#define LAYER_KEY_M04 0xF3
#define LAYER_KEY_M05 0xF4
#define LAYER_KEY_M06 0xF5
#define LAYER_KEY_M07 0xF6

#define NUM_POSSIBLE_MODES 6
#define MODE_KEY_M01 0xF9
#define MODE_KEY_M02 0xFA
#define MODE_KEY_M03 0xFB
#define MODE_KEY_M04 0xFC
#define MODE_KEY_M05 0xFD
#define MODE_KEY_M06 0xFE

typedef struct {
  uint8_t layer_keycodes[NUM_POSSIBLE_LAYERS];
  uint8_t mode_keycodes[NUM_POSSIBLE_MODES];
  uint8_t held_layers[NUM_POSSIBLE_LAYERS];
  uint8_t held_modes[NUM_POSSIBLE_MODES];
} mod_keys_t;

typedef enum {
  SET_LAYER,
  SET_MODE,
  SET_NONE
} layer_mode_t;

void initialize_layers(mod_keys_t *const layer);

layer_mode_t check_and_store_layer_mode(mod_keys_t *const layer, keymap_t *const keymap, uint8_t row, uint8_t col, int layer_index, int mode_index);

void set_layer_and_mode(matrix_state_t *const state, keymap_t *keymap, mod_keys_t *const layer);

void return_layer();

#endif // LAYER_PROCESSOR_H

#ifndef KEYMAP_PROCESSING_H
#define KEYMAP_PROCESSING_H

#include <stdint.h>

void initialize_keymap(void);

uint8_t return_keycode(uint8_t row, uint8_t column, uint8_t layer);

#endif // KEYMAP_PROCESSING_H

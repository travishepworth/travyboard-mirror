#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define MAX_KEYCODES 6

typedef struct {
  uint8_t keycodes[MAX_KEYCODES]; // Array to hold keycodes
  uint8_t count; // Number of keycodes in the array
} keycode_report_t;

void keyboard_init(void);

void process_matrix(keycode_report_t *report);

#endif // KEYBOARD_H

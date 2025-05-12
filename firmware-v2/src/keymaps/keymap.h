#ifndef KEYMAP_H
#define KEYMAP_H

#include "matrix.h"

enum layers {
  _base,
  _num,
  _fn1,
  _fn2
};

uint8_t default_keymap[sizeof(layers)][MATRIX_ROWS][MATRIX_COLS] {

};

#endif // KEYMAP_H

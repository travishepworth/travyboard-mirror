// debounce.c
#include "debounce.h"
#include "matrix.h"
#include <string.h>

// Initialize debounce state
void debounce_init(debounce_state_t *debounce) {
  memset(debounce, 0, sizeof(debounce_state_t));
}

// High-performance debounce function
void debounce_matrix(matrix_state_t *state, debounce_state_t *debounce) {
  uint32_t current_time = time_us_32();
  const uint32_t debounce_time_us = DEBOUNCE_TIME_MS * 1000;

  for (int i = 0; i < MATRIX_HALF_SIZE; i++) {
    uint8_t current_raw = state->state[i];
    uint8_t last_raw = debounce->raw_state[i];
    uint8_t debounced = debounce->debounced_state[i];

    // Detect state changes
    if (current_raw != last_raw) {
      if (current_raw) {
        // Key press detected
        debounce->debounced_state[i] = 1;
        debounce->release_timers[i] = 0; // Clear any pending release timer
      } else {
        // Key release detected - start debounce timer
        debounce->release_timers[i] = current_time;
      }
    }
    // Check for release debounce timeout
    else if (!current_raw && debounced && debounce->release_timers[i] != 0) {
      // Key is still released and we have a pending release timer
      if ((current_time - debounce->release_timers[i]) >= debounce_time_us) {
        // Debounce period has elapsed - confirm the release
        debounce->debounced_state[i] = 0;
        debounce->release_timers[i] = 0;
      }
    }

    // Update raw state tracking
    debounce->raw_state[i] = current_raw;

    // Write debounced state back to matrix
    state->state[i] = debounce->debounced_state[i];
  }
}

// Optional: Reset debounce state for specific key (useful for testing)
void debounce_reset_key(debounce_state_t *debounce, uint8_t key_index) {
  if (key_index < TOTAL_KEYS) {
    debounce->debounced_state[key_index] = 0;
    debounce->raw_state[key_index] = 0;
    debounce->release_timers[key_index] = 0;
  }
}

// Optional: Get debounce statistics (for debugging/tuning)
uint8_t debounce_get_pending_releases(debounce_state_t *debounce) {
  uint8_t count = 0;
  for (int i = 0; i < TOTAL_KEYS; i++) {
    if (debounce->release_timers[i] != 0) {
      count++;
    }
  }
  return count;
}

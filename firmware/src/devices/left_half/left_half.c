/*--Main--*/
#include "bsp/board_api.h"
#include <hardware/uart.h>

#include "debounce.h"

#include "matrix.h"
#include "uart.h"

static matrix_metadata_t metadata;
static uart_report_t uart_report;
static debounce_state_t debounce_state;
static matrix_state_t previous_state;

int main(void) {
  board_init();
  initialize_uart();

  select_matrix_backend();
  matrix_init(&metadata);
  initialize_uart_report(&uart_report);
  debounce_init(&debounce_state);
  matrix_clear(&previous_state);

  static uint32_t last_send_ms = 0;
  const uint32_t heartbeat_interval_ms = 5;
  const uint32_t interval_ms = 1;
  static uint32_t start_ms = 0;

  while (true) {
    uint32_t current_ms = board_millis();

    // Always scan matrix at consistent intervals
    if (current_ms - start_ms >= interval_ms) {
      start_ms = current_ms; // Reset to current time (prevents drift)

      matrix_state_t state;
      matrix_clear(&state);
      matrix_read(&state, &metadata);
      debounce_matrix(&state, &debounce_state);

      bool state_changed = !matrix_state_equals(&state, &previous_state);
      bool heartbeat_due = (current_ms - last_send_ms) >= heartbeat_interval_ms;

      if (state_changed || heartbeat_due) {
        pack_uart_report(&state, &uart_report);

        // Try to send, but update state regardless
        if (send_uart_report(&uart_report)) {
          last_send_ms = current_ms;
        }
        // Always update previous_state to prevent spam retries
        matrix_copy(&previous_state, state);
      }
    }

    // Optional: small sleep to prevent busy waiting
    sleep_us(100);
  }
  return 0;
};

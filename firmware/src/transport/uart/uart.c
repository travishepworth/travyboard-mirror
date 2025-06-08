#include "uart.h"
#include "hardware/uart.h"
#include <hardware/gpio.h>
#include <pico/time.h>
#include <string.h>

void initialize_uart(void) {
  // Initialize UART hardware
  uart_init(UART_ID, UART_BAUD_RATE);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

void initialize_uart_report(uart_report_t *report) {
  memset(report, 0, sizeof(uart_report_t));
}

void clear_uart_report(uart_report_t *report) {
  memset(report, 0, sizeof(uart_report_t));
}

void flush_uart(void) {
  while (uart_is_readable(UART_ID)) {
    (void)uart_getc(UART_ID);
  }
}

void pack_uart_report(matrix_state_t const *const state,
                      uart_report_t *report) {
  memset(report, 0, sizeof(uart_report_t));

  report->packet[0] = IDENTIFIER;

  int checksum = 0;
  for (int i = 0; i < TOTAL_KEYS_HALF; i++) {
    int byte_idx = 1 + (i / 8);
    int bit_pos = 7 - (i % 8);

    if (state->state[i]) {
      report->packet[byte_idx] |= (1 << bit_pos);
      checksum++;
    }
  }
  report->packet[UART_PACKET_SIZE - 1] = checksum & 0xFF;
}

bool send_uart_report(uart_report_t *report) {
  size_t packet_size = sizeof(report->packet);

  for (size_t i = 0; i < packet_size; i++) {
    if (!uart_is_writable(UART_ID)) {
      return false; // Can't send complete packet
    }
    uart_putc_raw(UART_ID, report->packet[i]);
  }
  return true;
}

bool receive_uart_report(matrix_state_t *const state, uart_report_t *report) {
  memset(report, 0, sizeof(uart_report_t));
  absolute_time_t deadline = make_timeout_time_ms(UART_TIMEOUT_MS);

  while (true) {
    if (!uart_is_readable(UART_ID)) {
      if (absolute_time_diff_us(get_absolute_time(), deadline) <= 0) {
        return false;
      }
      continue;
    }

    uint8_t byte = uart_getc(UART_ID);
    if (byte == IDENTIFIER) {
      report->packet[0] = byte;
      break;
    }
  }

  for (size_t i = 1; i < UART_PACKET_SIZE; ++i) {
    deadline = make_timeout_time_ms(UART_TIMEOUT_MS);

    while (!uart_is_readable(UART_ID)) {
      if (absolute_time_diff_us(get_absolute_time(), deadline) <= 0) {
        return false;
      }
    }
    report->packet[i] = uart_getc(UART_ID);
  }
  return unpack_uart_report(state, report);
}

bool unpack_uart_report(matrix_state_t *state, uart_report_t *const report) {
  if (report->packet[0] != IDENTIFIER) {
    return false;
  }

  memset(state->state, 0, sizeof(state->state));

  int checksum = 0;
  for (int i = 0; i < TOTAL_KEYS_HALF; i++) {
    int byte_idx = 1 + (i / 8);
    int bit_pos = 7 - (i % 8);

    if ((report->packet[byte_idx] >> bit_pos) & 1) {
      state->state[i] = 1;
      checksum++;
    }
  }
  uint8_t received_checksum = report->packet[UART_PACKET_SIZE - 1];
  return (checksum & 0xFF) == received_checksum;
}

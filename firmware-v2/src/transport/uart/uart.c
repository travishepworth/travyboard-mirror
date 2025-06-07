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

void pack_uart_report(matrix_state_t const *const state,
                      uart_report_t *report) {
  // Clear the report
  memset(report, 0, sizeof(uart_report_t));
  
  // Byte 0: Identifier
  report->packet[0] = IDENTIFIER;
  
  // Pack bitmap: compress state->state array into bitmap bytes
  int checksum = 0;
  for (int i = 0; i < TOTAL_KEYS_HALF; i++) {
    int byte_idx = 1 + (i / 8);        // Which bitmap byte (starting at index 1)
    int bit_pos = 7 - (i % 8);         // Bit position within byte (MSB first)
    
    if (state->state[i]) {
      report->packet[byte_idx] |= (1 << bit_pos);
      checksum++;
    }
  }
  
  // Final byte: store checksum
  report->packet[UART_PACKET_SIZE - 1] = checksum & 0xFF;
}

bool send_uart_report(uart_report_t *report) {
  for (int i = 0; i < sizeof(report->packet); i++) {
    if (!uart_is_writable(UART_ID)) {
      return false;
    }
    uart_putc_raw(UART_ID, report->packet[i]);
  }
  return true;
}

void flush_uart(void) {
  while (uart_is_readable(UART_ID)) {
    (void)uart_getc(UART_ID);
  }
}

bool receive_uart_report(matrix_state_t *const state, uart_report_t *report) {
  memset(report, 0, sizeof(uart_report_t));
  size_t i = 0;
  absolute_time_t deadline = make_timeout_time_ms(UART_TIMEOUT_MS);
  
  while (i < UART_PACKET_SIZE) {
    if (!uart_is_readable(UART_ID)) {
      // Check if we've timed out
      if (absolute_time_diff_us(get_absolute_time(), deadline) <= 0) {
        return false; // Timeout
      }
      continue; // Still waiting for data
    }
    
    uint8_t byte = uart_getc(UART_ID);
    
    // Validate identifier on first byte
    if (i == 0) {
      if (byte != IDENTIFIER) {
        return false;
      }
    }
    
    report->packet[i++] = byte;
    deadline = make_timeout_time_ms(UART_TIMEOUT_MS); // Reset timeout on each byte
  }
  
  return unpack_uart_report(state, report);
}

bool unpack_uart_report(matrix_state_t *state, uart_report_t *const report) {
  // Validate identifier
  if (report->packet[0] != IDENTIFIER) {
    return false;
  }
  
  // Clear the state
  memset(state->state, 0, sizeof(state->state));
  
  // Extract bitmap from bytes 1 to BITMAP_BYTES
  int checksum = 0;
  for (int i = 0; i < TOTAL_KEYS_HALF; i++) {
    int byte_idx = 1 + (i / 8);        // Which bitmap byte (starting at index 1)
    int bit_pos = 7 - (i % 8);         // Bit position within byte (MSB first)
    
    // Extract bit and store in state
    if ((report->packet[byte_idx] >> bit_pos) & 1) {
      state->state[i] = 1;
      checksum++;
    }
  }
  
  // Verify checksum (last byte of packet)
  uint8_t received_checksum = report->packet[UART_PACKET_SIZE - 1];
  return (checksum & 0xFF) == received_checksum;
}

#ifndef UART_H
#define UART_H

#include "hardware/uart.h"
#include "matrix.h"
#include "default.h"

#define UART_ID uart0
#define UART_BAUD_RATE 115200

#define UART_TIMEOUT_MS 10 // Timeout for receiving UART data in milliseconds
#define BITMAP_BTYES ((TOTAL_KEYS_HALF + 7) / 8)
#define UART_PACKET_SIZE (1 + BITMAP_BTYES + 1)

#define IDENTIFIER 0x42 // Example identifier, can be changed, doesn't really matter, avoid something that could be triggered by a keypress

#define UART_TX_PIN 0 
#define UART_RX_PIN 1

// TODO, make this a dynamic size packet, not locked to 4x7

typedef struct {
  uint8_t packet[UART_PACKET_SIZE];
  uint8_t checksum;
} uart_report_t;

void initialize_uart(void);

void initialize_uart_report(uart_report_t *report);
void clear_uart_report(uart_report_t *report);

void pack_uart_report(matrix_state_t const *const state, uart_report_t *report);
bool send_uart_report(uart_report_t *report);
void flush_uart(void);
bool receive_uart_report(matrix_state_t *const state, uart_report_t *report);
bool unpack_uart_report(matrix_state_t *const state, uart_report_t *const report);

#endif // UART_H

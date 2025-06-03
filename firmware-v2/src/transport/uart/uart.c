#include "uart.h"
#include "default.h"
#include "hardware/uart.h"
#include "matrix.h"
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
    // Initialize UART report structure
    memset(report, 0, sizeof(uart_report_t));
}

void pack_uart_report(matrix_state_t const *const state, uart_report_t *report) {
    // Clear the report
    memset(report, 0, sizeof(uart_report_t));
    
    // Byte 0: Identifier (bits 7-4) + first 4 bitmap bits (bits 3-0)
    report->packet[0] = (IDENTIFIER << 4) | 
                        (state->state[0] << 3) | 
                        (state->state[1] << 2) | 
                        (state->state[2] << 1) | 
                        state->state[3];
    
    // Bytes 1-3: Pack remaining 24 bits (keys 4-27)
    for (int i = 0; i < 24; i++) {
        int byte_idx = 1 + (i / 8);        // Which byte (1, 2, or 3)
        int bit_pos = 7 - (i % 8);         // Bit position within byte (7 down to 0)
        
        if (state->state[4 + i]) {
            report->packet[byte_idx] |= (1 << bit_pos);
        }
    }
    
    // Byte 4: Calculate checksum (bits 3-0), preserve upper nibble
    int checksum = 0;
    for (int i = 0; i < 28; i++) {
        if (state->state[i]) {
            checksum++;
        }
    }
    report->packet[4] = (report->packet[4] & 0xF0) | (checksum & 0x0F);
    report->checksum = checksum;
}

void send_uart_report(uart_report_t *report) {
    // Send the report over UART
    uart_write_blocking(UART_ID, report->packet, sizeof(report->packet));
}

bool recieve_uart_report(matrix_state_t *const state, uart_report_t *report) {
    memset(report, 0, sizeof(uart_report_t));
    size_t i = 0;
    absolute_time_t deadline = make_timeout_time_ms(UART_TIMEOUT_MS);
    
    while (i < UART_PACKET_SIZE) {
        if (!uart_is_readable(UART_ID)) {
            // Check if we've timed out
            if (absolute_time_diff_us(get_absolute_time(), deadline) <= 0) {
                return false; // Timeout
            }
            continue;  // Still waiting for data
        }
        
        uint8_t byte = uart_getc(UART_ID);
        if (i == 0) {
            // Sync on valid identifier in high nibble
            if ((byte & 0xF0) != (IDENTIFIER << 4)) {  // FIXED: Compare shifted identifier
                continue;  // Skip invalid start
            }
        }
        report->packet[i++] = byte;
        // Reset timeout after receiving a byte to allow full packet
        deadline = make_timeout_time_ms(UART_TIMEOUT_MS);
    }
    
    return unpack_uart_report(state, report);
}

bool unpack_uart_report(matrix_state_t *state, uart_report_t *const report) {
    // Verify identifier in upper nibble
    if ((report->packet[0] & 0xF0) != (IDENTIFIER << 4)) {  // FIXED: Compare shifted identifier
        return false;
    }
    
    // Clear the state
    memset(state->state, 0, 28);
    
    // Extract first 4 bits from byte 0 (bits 3-0)
    state->state[0] = (report->packet[0] >> 3) & 1;
    state->state[1] = (report->packet[0] >> 2) & 1;
    state->state[2] = (report->packet[0] >> 1) & 1;
    state->state[3] = report->packet[0] & 1;
    
    // Extract remaining 24 bits from bytes 1-3
    for (int i = 0; i < 24; i++) {
        int byte_idx = 1 + (i / 8);        // Which byte (1, 2, or 3)
        int bit_pos = 7 - (i % 8);         // Bit position within byte (7 down to 0)
        
        state->state[4 + i] = (report->packet[byte_idx] >> bit_pos) & 1;
    }
    
    // Verify checksum
    int expected_checksum = 0;
    for (int i = 0; i < 28; i++) {
        if (state->state[i]) {
            expected_checksum++;
        }
    }
    uint8_t received_checksum = report->packet[4] & 0x0F;
    
    return (expected_checksum & 0x0F) == received_checksum;
}

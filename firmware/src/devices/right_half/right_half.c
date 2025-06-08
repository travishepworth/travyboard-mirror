/*--Main--*/
#include "bsp/board_api.h"
#include "class/hid/hid.h"
#include "class/hid/hid_device.h"
#include <hardware/uart.h>

#include "debounce.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "keyboard.h"
#include "keymap.h"
#include "matrix.h"
#include "uart.h"

// Blink pattern
enum {
  BLINK_NOT_MOUNTED = 250, // device not mounted
  BLINK_MOUNTED = 1000,    // device mounted
  BLINK_SUSPENDED = 2500,  // device is suspended
};

void led_blinking_task(void); // define function prototype

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

static keymap_t keymap;
static keycode_report_t report;
static matrix_metadata_t metadata;
static debounce_state_t debounce_state;
static matrix_state_t left_state;
static uart_report_t uart_report;

int main(void) {
  board_init();
  tusb_init();

  initialize_uart();
  initialize_uart_report(&uart_report);

  keyboard_init(&keymap, &metadata);
  debounce_init(&debounce_state);
  matrix_clear(&left_state);

  const uint32_t interval_ms = 1;
  static uint32_t start_ms = 0;

  while (true) {
    tud_task();
    uint32_t current_ms = board_millis();
    if (current_ms - start_ms >= interval_ms) {
      start_ms = current_ms;

      matrix_state_t true_state;
      matrix_state_t right_state;
      matrix_clear(&true_state);
      matrix_clear(&right_state);

      // TODO: move to function, a bit too much logic for main
      if (uart_is_readable(UART_ID)) {
        clear_uart_report(&uart_report);
        matrix_state_t incoming_state;
        matrix_clear(&incoming_state);

        if (receive_uart_report(&incoming_state, &uart_report)) {
          matrix_copy(&left_state, incoming_state);
        }
      }
      matrix_copy(&true_state, left_state);

      matrix_read(&right_state, &metadata);
      debounce_matrix(&right_state, &debounce_state);
      // TODO: make this take 3 arguments, instead of copying right to true
      matrix_concatenate(&true_state, &right_state);

      matrix_convert(&true_state); // Extract array of indices from scan

      set_layer(&keymap, get_active_layer(true_state, &keymap.layer_indices));

      set_keycodes(&keymap, &report, &true_state);
      send_keyboard_report(&report);
    }
  }
  return 0;
};

// TODO: move to tusb.c

//--------------------------------------------------------------------+
// USB HID TINYUSB FUNCTIONS
//--------------------------------------------------------------------+

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report,
                                uint16_t len) {
  // not implemented, we only send REPORT_ID_KEYBOARD
  (void)instance;
  (void)len;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
  // TODO not Implemented
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
  (void)instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT) {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD) {
      // bufsize should be (at least) 1
      if (bufsize < 1)
        return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        board_led_write(true);
      } else {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) { blink_interval_ms = BLINK_MOUNTED; }

// Invoked when device is unmounted
void tud_umount_cb(void) { blink_interval_ms = BLINK_NOT_MOUNTED; }

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from
// bus
void tud_suspend_cb(bool remote_wakeup_en) {
  (void)remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) { blink_interval_ms = BLINK_MOUNTED; }

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void) {
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // blink is disabled
  if (!blink_interval_ms)
    return;

  // Blink every interval ms
  if (board_millis() - start_ms < blink_interval_ms)
    return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}

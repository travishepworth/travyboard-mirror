/*--Main--*/
#include "bsp/board_api.h"
#include "class/hid/hid.h"
#include "class/hid/hid_device.h"
#include <hardware/uart.h>

#include "debounce.h"
#include "tusb.h"
#include "usb_descriptors.h"

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

  while (true) {
    const uint32_t interval_ms = 1;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms) {
      continue; // not enough time
    }
    start_ms += interval_ms;

    matrix_state_t state;
    matrix_clear(&state);
    
    matrix_read(&state, &metadata);
    debounce_matrix(&state, &debounce_state);

    bool state_changed = !matrix_state_equals(&state, &previous_state);
    bool heartbeat_due = (board_millis() - last_send_ms) >= heartbeat_interval_ms;

    if (state_changed || heartbeat_due) {
      pack_uart_report(&state, &uart_report);
      if (send_uart_report(&uart_report)) {
        matrix_copy(&previous_state, state);
        last_send_ms = board_millis();
      }
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

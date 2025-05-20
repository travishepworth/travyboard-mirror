/*--Main--*/
#include "bsp/board_api.h"
#include "keyboard.h"
#include "class/hid/hid.h"

int main() {
  // Init libraries
  board_init();

  // Create required sctructs
  keymap_modes_t modes;
  keymap_t keymap;
  keycode_report_t report;
  mod_keys_t layer_and_mode;

  // Initialize the keyboard
  keyboard_init(&modes, &keymap, &layer_and_mode);

  // Initialize the keymap

  
  // Initialize uart
  

  // Process logic
  while (1) {

    // Todo, almost a complete setup for a single keyboard
    // Implement debounce, key sending, and layer handing
    // Need to figure out kinks with uart and keyboard report
    // Probably another function to read the other half?
    // How should layers be concatenated into a single keymap
    // with the halves translating accordingly
    //
    // Thinking probably something to merge the uart calls.
    process_matrix(&report, &keymap, &layer_and_mode);
    send_keyboard_report(&report);
  }
};

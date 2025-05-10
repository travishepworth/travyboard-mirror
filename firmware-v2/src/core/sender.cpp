#include "RF24.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

// RF pin definitions
#define CE_PIN 22
#define CSN_PIN 8
#define SPI_SCK 18
#define SPI_MOSI 19
#define SPI_MISO 16
#define IRQ_PIN 17

// button
#define BUTTON_PIN 15

// RF24 Pipe address
const uint64_t address = 0xF0F0F0F0E1LL;

// Initialize RF24 object
RF24 radio(CE_PIN, CSN_PIN);

void setup_rf24() {
  // Initialize SPI
  spi_init(spi0, 10 * 1000 * 1000); // Set SPI speed to 4 MHz
  gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
  gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI);
  gpio_set_function(SPI_MISO, GPIO_FUNC_SPI);

  // Initialize RF24
  if (!radio.begin()) {
    printf("RF24 module not found\n");
    while (1)
      ;
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setRetries(3, 5);
  radio.openWritingPipe(address);
  radio.stopListening();
}

int main() {
  // Initialize stdio
  stdio_init_all();
  sleep_ms(2000); // Wait for serial to initialize

  // Initialize button
  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_up(BUTTON_PIN);

  // Initialize RF24
  setup_rf24();
  printf("RF24 Sender Ready\n");

  bool last_button_state = true;
  bool toggle_state = false;

  // Main loop
  while (1) {
    // Read button state
    bool current_button_state = gpio_get(BUTTON_PIN);

    if (last_button_state && !current_button_state) {
      toggle_state = !toggle_state; // Toggle state on button press

      radio.write(&toggle_state, sizeof(toggle_state));
    }
  }
  sleep_ms(50); // Debounce delay
}

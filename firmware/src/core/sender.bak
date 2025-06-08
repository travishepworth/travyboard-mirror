#include "RF24.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

// RF pin definitions
#define CE_PIN 22
#define CSN_PIN 8
#define SPI_SCK 18
#define SPI_MOSI 19
#define SPI_MISO 16
#define IRQ_PIN 17

// Button and LED pins
#define BUTTON_PIN 15
#define LED_PIN 27

// RF24 Pipe address
const uint64_t address = 0xA7B3C5D9E2LL;

// Initialize RF24 object
RF24 radio(CE_PIN, CSN_PIN);

// Payload
bool toggle_state = false;

bool setup() {
  // Initialize button
  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_up(BUTTON_PIN);

  // Initialize LED
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, 0); // LED initially off

  // Initialize SPI
  spi_init(spi0, 10 * 1000 * 1000); // Set SPI speed to 10 MHz
  gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
  gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI);
  gpio_set_function(SPI_MISO, GPIO_FUNC_SPI);

  // Initialize RF24
  if (!radio.begin()) {
    return false;
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setRetries(3, 5);
  radio.setPayloadSize(sizeof(toggle_state));
  radio.openWritingPipe(address);
  radio.setChannel(76); // Set channel to 76
  radio.stopListening();

  return true;
}

void loop() {
  static bool last_button_state = false;
  bool current_button_state = !gpio_get(BUTTON_PIN); // Active-low button

  // Detect button press (rising edge)
  if (!last_button_state && current_button_state) {
    toggle_state = !toggle_state;                     // Toggle state
    gpio_put(LED_PIN, toggle_state);                  // Update LED
    radio.write(&toggle_state, sizeof(toggle_state)); // Send to receiver
  }

  last_button_state = current_button_state;
  sleep_ms(50); // Debounce delay
}

int main() {
  stdio_init_all(); // Initialize IO (required for Pico)

  while (!setup()) {
    // Retry initialization if RF24 fails
  }

  while (true) {
    loop();
  }

  return 0;
}

#include "Adafruit_TinyUSB.h"

// the GPIO Pin I'm using for my contact
#define BUZZER_PIN 2

Adafruit_USBD_WebUSB webUSB;
// define the URL where the system notification should point to
WEBUSB_URL_DEF(landingPage, 1, "webusb-buzzer.nico.dev");

// default button state
byte prevButtonState = LOW;

void setup() {
#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // required for the RP2040 chip
  TinyUSB_Device_Init(0);
#endif

  webUSB.setLandingPage(&landingPage);
  webUSB.begin();

  // set the pin mode for the contact pin.
  // the INPUT_PULLDOWN allows us to read a constant LOW state instead of a floating signal.
  pinMode(BUZZER_PIN, INPUT_PULLDOWN);

  // I'm using the builtin led on the board for a visual feedback. So I set the pinMode here as well
  pinMode(LED_BUILTIN, OUTPUT);

  // wait until device mounted
  while ( !TinyUSBDevice.mounted() ) delay(1);
}

void loop() {
  // read the new button state
  byte newButtonState = digitalRead(BUZZER_PIN);

  // check if the state has changed
  if (prevButtonState != newButtonState) {

    // show the visual LED feedback
    digitalWrite(LED_BUILTIN, newButtonState);

    // create a new array of bytes with one element, the state of the button
    char charArray[1];
    charArray[0] = newButtonState;

    // write one byte to the WebUSB connection
    webUSB.write(charArray, 1);

    // set the new button state as pervious button state
    prevButtonState = newButtonState;
  }

  // the delay helps to have a clean read. I noticed some flickering without the 50ms delay.
  delay(50);
}

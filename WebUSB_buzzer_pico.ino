#include "Adafruit_TinyUSB.h"

#define BUZZER_PIN 2

Adafruit_USBD_WebUSB webUSB;
WEBUSB_URL_DEF(landingPage, 1, "webusb-buzzer.nico.dev");

byte prevButtonState = LOW;

void setup() {
#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  TinyUSB_Device_Init(0);
#endif

  webUSB.setStringDescriptor("WebUSB Buzzer");
  webUSB.setLandingPage(&landingPage);
  webUSB.begin();

  pinMode(BUZZER_PIN, INPUT_PULLDOWN);
  pinMode(LED_BUILTIN, OUTPUT);

  while ( !TinyUSBDevice.mounted() ) delay(1);
}

void loop() {
  byte newButtonState = digitalRead(BUZZER_PIN);

  if (prevButtonState != newButtonState) {
    digitalWrite(LED_BUILTIN, newButtonState);
    char charArray[1];
    charArray[0] = newButtonState;
    webUSB.write(charArray, 1);
    prevButtonState = newButtonState;
  }
  delay(50);
}

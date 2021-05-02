#include <led.h>
#include <Particle.h>
#include <chrono>

LEDStatus ledTesting(RGB_COLOR_BLUE, LED_PATTERN_FADE, LED_PRIORITY_NORMAL);
LEDStatus ledPass(RGB_COLOR_GREEN, LED_PATTERN_SOLID, LED_PRIORITY_NORMAL);
LEDStatus ledFail(RGB_COLOR_RED, LED_PATTERN_SOLID, LED_PRIORITY_NORMAL);

namespace led {
  void setup() {
    pinMode(TESTING_LED, OUTPUT);
    pinMode(PASS_LED, OUTPUT);
    pinMode(FAIL_LED, OUTPUT);
  }
}

void resetLeds() {
  setLed(PASS_LED, false);
  setLed(FAIL_LED, false);
  setLed(TESTING_LED, false);
}

void resetTestingLed() {
  setLed(PASS_LED, false);
  setLed(FAIL_LED, false);
  setLed(TESTING_LED, true);
}

void setLed(pin_t LED, bool on) {
  if (on) {
    digitalWrite(LED, HIGH);
    switch (LED) {
      case TESTING_LED:
        ledTesting.setActive(true);
        break;
      case PASS_LED:
        ledPass.setActive(true);
        break;
      case FAIL_LED:
        ledFail.setActive(true);
        break;
      default:
        break;
    }
  } else {
    digitalWrite(LED, LOW);
    switch (LED) {
      case TESTING_LED:
        ledTesting.setActive(false);
        break;
      case PASS_LED:
        ledPass.setActive(false);
        break;
      case FAIL_LED:
        ledFail.setActive(false);
        break;
      default:
        break;
    }
  }
}

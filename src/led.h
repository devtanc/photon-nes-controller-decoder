#ifndef LED_H
#define LED_H
#include <chrono>
#include <Particle.h>

const pin_t TESTING_LED = A1;
const pin_t PASS_LED = A2;
const pin_t FAIL_LED = A3;

namespace led {
    void setup();
}

void resetLeds();
void resetTestingLed();
void setLed(pin_t LED, bool on);

#endif /* LED_H */
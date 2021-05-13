#include <Particle.h>
#include <sn74register.h>

namespace sn74register {
  void enableOutput() {
    digitalWrite(ENABLE_OUTPUT, LOW);
  }

  void disableOutput() {
    digitalWrite(ENABLE_OUTPUT, HIGH);
  }

  void clearRegister() {
    digitalWrite(CLEAR, LOW);
    digitalWrite(CLEAR, HIGH);
  }

  void pulseHigh(pin_t pin) {
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
  }

  void pulseLow(pin_t pin) {
    digitalWrite(pin, LOW);
    digitalWrite(pin, HIGH);
  }

  void registerWriteHigh() {
    digitalWrite(DATA, HIGH);
    pulseHigh(SHIFT_CLOCK);
    digitalWrite(DATA, LOW);
  }

  void registerWriteLow() {
    digitalWrite(DATA, LOW);
    pulseHigh(SHIFT_CLOCK);
  }

  void writeRegister(int data, int method) {
    shiftOut(DATA, SHIFT_CLOCK, method, data);
    pulseHigh(LATCH);
  }

  void setup() {
    pinMode(DATA, OUTPUT);
    pinMode(ENABLE_OUTPUT, OUTPUT);
    pinMode(LATCH, OUTPUT);
    pinMode(SHIFT_CLOCK, OUTPUT);
    pinMode(CLEAR, OUTPUT);
    
    clearRegister();
    enableOutput();
    digitalWrite(LATCH, LOW);
  }
}
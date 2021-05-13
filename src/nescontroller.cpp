#include <nescontroller.h>
#include <Particle.h>

const int CLOCK_DELAY = 10;

const pin_t CLOCK = A3;
const pin_t LATCH = A4;
const pin_t DATA = A5;

typedef enum {
  A,
  B,
  SELECT,
  START,
  UP,
  DOWN,
  LEFT,
  RIGHT
} Button;

namespace nescontroller {
  void setup() {
    pinMode(CLOCK, OUTPUT);
    pinMode(LATCH, OUTPUT);
    pinMode(DATA, INPUT);
    digitalWrite(CLOCK, HIGH);
  }
}

int readRegister() {
  unsigned int data = 0b00000000;

  for (int i = 0; i < 8; i++) {
    int bit = digitalRead(DATA);
    data = setBit(data, i, bit);
    sendClockPulse();
  }

  return data;
}

void sendLatchPulse() {
  digitalWrite(LATCH, HIGH);
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(LATCH, LOW);
  delayMicroseconds(CLOCK_DELAY);
}

void sendClockPulse() {
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(CLOCK, LOW);
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(CLOCK, HIGH);
  delayMicroseconds(CLOCK_DELAY);
}

int setBit(unsigned int data, int bit, unsigned int value) {
  if (value == 0) return data;

  switch(bit) {
    case A:
      return data | 0b00000001;
    case B:
      return data | 0b00000010;
    case SELECT:
      return data | 0b00000100;
    case START:
      return data | 0b00001000;
    case UP:
      return data | 0b00010000;
    case DOWN:
      return data | 0b00100000;
    case LEFT:
      return data | 0b01000000;
    case RIGHT:
      return data | 0b10000000;
    default:
      return data;
  }
}

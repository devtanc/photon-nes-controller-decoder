#include <string>

const pin_t CLOCK = D0;
const pin_t LATCH = D1;
const pin_t DATA = D2;
const int CLOCK_DELAY = 10;
int loop_counter = 0;

bool TEST_BEGIN = false;
bool TEST_PASSED = false;

unsigned int registerData = 0b00000000;
unsigned int buttonResults = 0b00000000;

typedef enum {
  RELEASED,
  PRESSED
} ButtonState;

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

void setup() {
  Serial.begin(9600);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(DATA, INPUT);
  digitalWrite(CLOCK, HIGH);
}

void loop() {
  loop_counter = loop_counter + 1;
  registerData = 0;
  sendLatchPulse();
  registerData = readRegister();
  logData(registerData);
  delayMicroseconds(60);
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
  if (value == 0) {
    return data;
  }

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

void createBinaryString(unsigned int data, char result[]) {
  for (int i = 0; i < 8; i++) {
    if (data & 1) {
      result[i] = '1';
    } else {
      result[i] = '0';
    }
    data >>= 1;
  }
  result[8] = '\0';
}


void logData(unsigned int data) {
  char dataString[9];
  createBinaryString(data, dataString);
  Serial.printf("LOOP %d\n", loop_counter);
  Serial.println(dataString);
}

void resetButtonResults() {
  buttonResults = 0b00000000;
}
